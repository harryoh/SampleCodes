import os, sys
import urllib
import urllib2
from ipx_status import Status

__version_info__ = (0, 0, 1)
__version__ = '.'.join(unicode(item) for item in __version_info__)

_Usage = "%prog --login_id <id for targetip> --login_password <password for targetip> <Target IP>"
_Description = "Get OProfile results"

class OprofileError(Exception): pass

def parseOptions(arguments):
    assert arguments is not None
    from optparse import OptionParser

    parser = OptionParser(usage=_Usage, description=_Description,
                          version="%prog " + __version__)
    parser.add_option("-i", "--login_id", dest="login_id",
                  help="Login id for target camera")
    parser.add_option("-p", "--login_password", dest="login_password",
                  help="Login password for target camera")
    (options, others) = parser.parse_args(arguments[1:])

    if len(others) == 0:
        parser.error("Target IP must be appointed.")
    return (options, others[0])

class OProfile:
    def __init__(self, target_ip, login_id, login_password):
        self.status = Status(target_ip, login_id, login_password)
        if login_id is None: login_id = 'root'
        if login_password is None: login_password = 'pass'
        self.target_ip = target_ip
        self.login_id = login_id
        self.login_password = login_password
        self.status.PrintInformation(self.status.GetStatus('brand'))
        print

    def Start(self):
        self.status.PrintStartMessage('Start OProfile Test')
        authinfo = urllib2.HTTPPasswordMgrWithDefaultRealm()
        authinfo.add_password(None, self.target_ip, self.login_id, self.login_password)
        handler = urllib2.HTTPBasicAuthHandler(authinfo)
        opener = urllib2.build_opener(handler)
        urllib2.install_opener(opener)

        encoded_args = urllib.urlencode({ 'command' : 'oprofile-start.sh' })
        url = 'http://%s/uapi-cgi/testcmd.cgi?%s' % (self.target_ip, encoded_args)

        try:
            result = urllib2.urlopen(url, timeout=300).read()
            if not os.path.exists('result'): os.makedirs('result')
            fp = file('result/oprofile-%s.txt' % self.target_ip, 'w')
            fp.write(result)
            fp.close()
            print result
        except Exception, e:
            raise OprofileError(e)

        self.status.PrintDoneMessage()

def main(arguments=None):
    if arguments is None:
        arguments = sys.argv
    options, target_ip = parseOptions(arguments)
    oprofile = OProfile(target_ip, options.login_id, options.login_password)
    oprofile.Start()
    return 0

if __name__ == "__main__":
    exitCode = main()
    sys.exit(exitCode)
