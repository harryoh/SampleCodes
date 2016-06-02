import sys
import time
import glob
import requests
from ipx_status import Status

__version_info__ = (0, 0, 1)
__version__ = '.'.join(unicode(item) for item in __version_info__)

_Usage = "%prog --firmware <firmware(Ant-Pattern)> --targetip <target_ip>"
_Description = "Upgrade a new firmware to IPX camera"

class UpgradeError(Exception): pass
class RebootError(Exception): pass

def parseOptions(arguments):
    assert arguments is not None
    from optparse import OptionParser
    global login_id
    global login_password

    parser = OptionParser(usage=_Usage, description=_Description,
                          version="%prog " + __version__)
    parser.add_option("-f", "--firmware", dest="firmwarename",
                      help="A new firmware that will be upgraded",
                      metavar="AntFilePattern")
    parser.add_option("-l", "--location", dest="location", default=".",
                      help="Specify the directory (including path) where \
                      the firmware is")
    parser.add_option("-t", "--targetip", dest="target_ip",
                      help="Target camera that you wanted to upgrade")
    parser.add_option("-i", "--login_id", dest="login_id",
                  help="Login id for target camera")
    parser.add_option("-p", "--login_password", dest="login_password",
                  help="Login password for target camera")
    (options, others) = parser.parse_args(arguments)

    if options.firmwarename is None:
        parser.error("option --firmware must be set")

    firmware_pattern = glob.glob(options.firmwarename)
    if len(firmware_pattern) == 0:
        parser.error("Can't find any firmware")
    if options.target_ip is None:
        parser.error("option --targetip must be set")
    return (options, firmware_pattern[0])

class Upgrader:
    def __init__(self, target_ip, firmwarename, login_id='root', login_password='pass'):
        assert target_ip is not None
        assert firmwarename is not None
        self.target_ip = target_ip
        self.firmwarename = firmwarename
        self.login_id = login_id
        self.login_password = login_password
        self.status = Status(self.target_ip, self.login_id, self.login_password)
        self.current = self.status.GetStatus('brand')
        self.PrintInformation()

    def PrintInformation(self):
        print "Start Firmware upgrade"
        print "Target IP: " + self.target_ip
        print "Serial: " + self.current['brand']['model']['serial']
        print 'Firmware: ' + self.current['brand']['version']['firmware']
        if 'revision' in self.current['brand']['version']:
            print 'Revision: ' + self.current['brand']['version']['revision']
        else:
            self.current['brand']['version']['revision'] = ''
        print

    def start(self):
        url='http://'+self.target_ip+'/uapi-cgi/fwupload.cgi'
        files={'Filedata':open(self.firmwarename,'rb')}
        r = requests.post(url, files=files,auth=(self.login_id, self.login_password))
        if r.status_code != 200:
            raise UpgradeError(u'HTTP Status Code Error: %r' % r.status_code)

        print "Waiting device reboot..."
        time.sleep(200)
        try:
            newversion = self.status.GetStatus('brand', timeout=30)
        except Exception, e:
            raise RebootError(e)

        print 'New Firmware Version: ' + newversion['brand']['version']['firmware']
        if 'revision' in newversion['brand']['version']:
            print 'Revision: ' + newversion['brand']['version']['revision']
        else:
            newversion['brand']['version']['revision'] = ''
        if self.current['brand']['version']['firmware'] == newversion['brand']['version']['firmware'] and \
           self.current['brand']['version']['revision'] == newversion['brand']['version']['revision']:
            print >> sys.stderr, '** Warning: New Version is same with previous version.'

def main(arguments=None):
    if arguments is None:
        arguments = sys.argv
    options, firmwarename = parseOptions(arguments)
    upgrader = Upgrader(options.target_ip, firmwarename, options.login_id, options.login_password)
    upgrader.start()
    print "Done"
    return 0

if __name__ == "__main__":
    exitCode = main()
    sys.exit(exitCode)