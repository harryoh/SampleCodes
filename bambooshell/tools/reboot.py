import sys
import urllib2

__version_info__ = (0, 0, 1)
__version__ = '.'.join(unicode(item) for item in __version_info__)

_Usage = "%prog --login_id <id for targetip> --login_password <password for targetip> <Target IP>"
_Description = "Reboot a camera"

class RequestError(Exception): pass

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

class Reboot:
    def __init__(self, target_ip, login_id, login_password):
        if login_id is None: login_id = 'root'
        if login_password is None: login_password = 'pass'
        self.target_ip = target_ip
        self.login_id = login_id
        self.login_password = login_password

    def Run(self):
        authinfo = urllib2.HTTPPasswordMgrWithDefaultRealm()
        authinfo.add_password(None, self.target_ip, self.login_id, self.login_password)
        handler = urllib2.HTTPBasicAuthHandler(authinfo)
        opener = urllib2.build_opener(handler)
        urllib2.install_opener(opener)

        url = 'http://%s/uapi-cgi/reboot.cgi' % self.target_ip
        try:
            urllib2.urlopen(url, timeout=10)
        except Exception, e:
            raise RequestError(e)

def main(arguments=None):
    if arguments is None:
        arguments = sys.argv
    options, target_ip = parseOptions(arguments)
    reboot = Reboot(target_ip, options.login_id, options.login_password)
    reboot.Run()
    return 0

if __name__ == "__main__":
    exitCode = main()
    sys.exit(exitCode)