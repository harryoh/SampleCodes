import sys
import time
import requests
from ipx_status import Status

__version_info__ = (0, 0, 1)
__version__ = '.'.join(unicode(item) for item in __version_info__)

_Usage = "%prog --targetip <target_ip> --login_id <id for targetip> --login_password <password for targetip>"
_Description = "Makes IPX camera to factory default"

class FactoryError(Exception): pass
class RebootError(Exception): pass

def parseOptions(arguments):
    assert arguments is not None
    from optparse import OptionParser

    parser = OptionParser(usage=_Usage, description=_Description,
                          version="%prog " + __version__)
    parser.add_option("-t", "--targetip", dest="target_ip",
                      help="Target camera that you wanted to factory default")
    parser.add_option("-i", "--login_id", dest="login_id",
                  help="Login id for target camera")
    parser.add_option("-p", "--login_password", dest="login_password",
                  help="Login password for target camera")
    (options, others) = parser.parse_args(arguments)

    if options.target_ip is None:
        parser.error("option --targetip must be set")
    return options

class Factory:
    def __init__(self, target_ip, login_id='root', login_password='pass'):
        assert target_ip is not None
        self.target_ip = target_ip
        self.login_id = login_id
        self.login_password = login_password
        self.status = Status(self.target_ip, login_id, login_password)
        self.current = self.status.GetStatus('brand')
        self.PrintInformation()

    def PrintInformation(self):
        print "Start Factory default"
        print "Target IP: " + self.target_ip
        print "Serial: " + self.current['brand']['model']['serial']
        print 'Firmware: ' + self.current['brand']['version']['firmware']
        if 'revision' in self.current['brand']['version']:
            print 'Revision: ' + self.current['brand']['version']['revision']
        print

    def start(self):
        url='http://'+self.target_ip+'/uapi-cgi/factory.cgi?preserve=network'
        r = requests.get(url, auth=(self.login_id, self.login_password))
        if r.status_code != 200:
            raise FactoryError(u'HTTP Status Code Error: %r' % r.status_code)

        print "Waiting device reboot..."
        time.sleep(100)
        try:
            self.status.GetStatus('brand', timeout=30)
        except Exception:
            raise RebootError

def main(arguments=None):
    if arguments is None:
        arguments = sys.argv
    options = parseOptions(arguments)
    factory = Factory(options.target_ip, options.login_id, options.login_password)
    factory.start()
    print "Done"
    return 0

if __name__ == "__main__":
    exitCode = main()
    sys.exit(exitCode)