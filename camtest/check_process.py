import os, sys
import urllib
import urllib2
import shutil
import json
from ipx_status import Status

__version_info__ = (0, 0, 1)
__version__ = '.'.join(unicode(item) for item in __version_info__)

_Usage = "%prog --login_id <id for targetip> --login_password <password for targetip> <Target IP>"
_Description = "Check Status of a camera"

class FindingError(Exception): pass
class ProcessError(Exception): pass

def parseOptions(arguments):
    assert arguments is not None
    from optparse import OptionParser

    parser = OptionParser(usage=_Usage, description=_Description,
                          version="%prog " + __version__)
    parser.add_option("-i", "--login_id", dest="login_id",
                  help="Login id for target camera")
    parser.add_option("-p", "--login_password", dest="login_password",
                  help="Login password for target camera")
    parser.add_option("-P", "--process_file", dest="process_file",
                  default='pidlist/default.pidlist', help="Process List file")
    (options, others) = parser.parse_args(arguments[1:])

    if len(others) == 0:
        parser.error("Target IP must be appointed.")
    return (options, others[0])

class CheckProcess:
    def __init__(self, target_ip, login_id, login_password):
        self.status = Status(target_ip, login_id, login_password)
        if login_id is None: login_id = 'root'
        if login_password is None: login_password = 'pass'
        self.target_ip = target_ip
        self.login_id = login_id
        self.login_password = login_password
        self.status.PrintInformation(self.status.GetStatus('brand'))
        print

    def Start(self, process_file):
        self.status.PrintStartMessage(' Start Check Processes')
        authinfo = urllib2.HTTPPasswordMgrWithDefaultRealm()
        authinfo.add_password(None, self.target_ip, self.login_id, self.login_password)
        handler = urllib2.HTTPBasicAuthHandler(authinfo)
        opener = urllib2.build_opener(handler)
        urllib2.install_opener(opener)

        process_list = eval(open(process_file, 'r').read())
        print 'Using \'%s\' file for pidlist' % process_file
        for key, value in process_list.iteritems() :
            find_flag = False
            print 'Finding [%s]...' % key,
            for process in key.split('|'):
                encoded_args = urllib.urlencode({ 'command' : 'pidof %s' % process })
                url = 'http://%s/uapi-cgi/testcmd.cgi?%s' % (self.target_ip, encoded_args)

                try:
                    result = urllib2.urlopen(url, timeout=5).read()
                    if result is not '':
                        find_flag = True
                        break;
                except Exception, e:
                    raise ProcessError(e)

            if find_flag is True:
                print "OK"
            else:
                raise FindingError('Couldn\'t find %s in processes' % key)

        if not os.path.exists('result'): os.makedirs('result')
        fp = file('result/process_list-%s.txt' % self.target_ip, 'w')
        fp.write(json.dumps(process_list, indent=1))
        fp.close()

        self.status.PrintDoneMessage()

def main(arguments=None):
    if arguments is None:
        arguments = sys.argv
    options, target_ip = parseOptions(arguments)
    test = CheckProcess(target_ip, options.login_id, options.login_password)
    test.Start(options.process_file)

    return 0

if __name__ == "__main__":
    exitCode = main()
    sys.exit(exitCode)
