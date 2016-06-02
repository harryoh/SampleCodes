import os, sys
import urllib
import urllib2
from ipx_status import Status

__version_info__ = (0, 0, 1)
__version__ = '.'.join(unicode(item) for item in __version_info__)

_Usage = "%prog --login_id <id for targetip> --login_password <password for targetip> --file <filename> <Target IP>"
_Description = "Check 'tmp/wsdd.lock' file"

class FindError(Exception): pass

def parseOptions(arguments):
    assert arguments is not None
    from optparse import OptionParser

    parser = OptionParser(usage=_Usage, description=_Description,
                          version="%prog " + __version__)
    parser.add_option("-i", "--login_id", dest="login_id",
                  help="Login id for target camera")
    parser.add_option("-p", "--login_password", dest="login_password",
                  help="Login password for target camera")
    parser.add_option("-f", "--file", dest="filename",
                  help="File full path you want to check")
    (options, others) = parser.parse_args(arguments[1:])

    assert options.filename is not None
    if len(others) == 0:
        parser.error("Target IP must be appointed.")
    return (options, others[0])

class FileCheck:
    def __init__(self, target_ip, login_id, login_password):
        self.status = Status(target_ip, login_id, login_password)
        if login_id is None: login_id = 'root'
        if login_password is None: login_password = 'pass'
        self.target_ip = target_ip
        self.login_id = login_id
        self.login_password = login_password
        self.status.PrintInformation(self.status.GetStatus('brand'))
        print

    def Start(self, filename):
        self.status.PrintStartMessage('Start FileCheck Test')
        authinfo = urllib2.HTTPPasswordMgrWithDefaultRealm()
        authinfo.add_password(None, self.target_ip, self.login_id, self.login_password)
        handler = urllib2.HTTPBasicAuthHandler(authinfo)
        opener = urllib2.build_opener(handler)
        urllib2.install_opener(opener)

        args = urllib.urlencode({ 'command' : 'ls %s' % filename })
        url = 'http://%s/uapi-cgi/testcmd.cgi?%s' % (self.target_ip, args)

        try:
            print 'Finding [%s]...' % filename,
            result = urllib2.urlopen(url, timeout=5).read()
            if result is not '':
                print "OK"
            else:
                raise FindError('Couldn\'t find %s.' % filename)
        except Exception, e:
            raise FindError(e)

        self.status.PrintDoneMessage()

def main(arguments=None):
    if arguments is None:
        arguments = sys.argv
    options, target_ip = parseOptions(arguments)
    filecheck = FileCheck(target_ip, options.login_id, options.login_password)
    filecheck.Start(options.filename)
    return 0

if __name__ == "__main__":
    exitCode = main()
    sys.exit(exitCode)
