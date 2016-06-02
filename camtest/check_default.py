import os, sys, re
import urllib, urllib2
import string
import json
import shutil
from xml.etree.ElementTree import parse
from ipx_status import Status

__version_info__ = (1, 0, 0)
__version__ = '.'.join(unicode(item) for item in __version_info__)

_Usage = "%prog --login_id <id for targetip> --login_password <password for targetip> --default_file <Default Value file> <Target IP>"
_Description = "Check default value of a camera"

class DefaultValueError(Exception): pass

def parseOptions(arguments):
    assert arguments is not None
    from optparse import OptionParser

    parser = OptionParser(usage=_Usage, description=_Description,
                          version="%prog " + __version__)
    parser.add_option("-i", "--login_id", dest="login_id",
                  help="Login id for target camera")
    parser.add_option("-p", "--login_password", dest="login_password",
                  help="Login password for target camera")
    parser.add_option("-f", "--default_file", dest="default_file",
                  help="Default value file")
    (options, others) = parser.parse_args(arguments[1:])

    if options.default_file is None:
        parser.error("Default File should be appointed.")
    if os.path.isfile(options.default_file) == False:
        parser.error("Couldn't file default file (%s)" % options.default_file)

    if len(others) == 0:
        parser.error("Target IP must be appointed.")
    return (options, others[0])

class CheckDefaultValues:
    def __init__(self, target_ip, login_id, login_password):
        if login_id is None: login_id = 'root'
        if login_password is None: login_password = 'pass'
        self.status = Status(target_ip, login_id, login_password)
        self.target_ip = target_ip
        self.login_id = login_id
        self.login_password = login_password


#        self.exception_re = []
#        for reg in self.exception_api:
#            self.exception_re.append(re.compile(reg))

        self.status.PrintInformation(self.status.GetStatus('brand'))
        print

    def Start(self, default_file):
        assert default_file is not None
        self.status.PrintStartMessage(' Start Check default values')
        source = parse(default_file).getroot()
        url = source.find("information").findtext("url").strip()
        if url is not None:
            self.url = "http://%s%s" % (self.target_ip, url)

        self.exception_re = []
        for reg in source.find("ignore").findtext("node").strip().splitlines(True):
            self.exception_re.append(re.compile(reg.strip()))

        self.exception_if_re = []
        for reg in source.find("ignore").findtext("check_if_exist").strip().splitlines(True):
            self.exception_if_re.append(re.compile(reg.strip()))

        source_api = self.apiTodict(source.findtext("api").strip())

        authinfo = urllib2.HTTPPasswordMgrWithDefaultRealm()
        authinfo.add_password(None, self.target_ip, self.login_id, self.login_password)
        handler = urllib2.HTTPBasicAuthHandler(authinfo)
        opener = urllib2.build_opener(handler)
        urllib2.install_opener(opener)
        result = urllib2.urlopen(self.url, timeout=10)
        desc_api = self.apiTodict(result.read());
        result.close()

        check_api = source_api.copy()
        for key, value in source_api.iteritems():
            is_exception = False
            for reg in self.exception_re:
                if reg.search(key) is not None:
                    is_exception = True
                    del check_api[key]
                    break
            for reg in self.exception_if_re:
                if reg.search(key) is not None:
                    if desc_api.has_key(key) is False:
                        is_exception = True
                        break
            if is_exception == True: continue

            print "%s = %s" % (key, desc_api[key])
            if desc_api[key] != value:
                raise DefaultValueError("'%s' should be '%s'" % (key, value))
                break
            else:
                del desc_api[key]

        if not os.path.exists('result'): os.makedirs('result')
        fp = file('result/default_%s.txt' % default_file.split("/")[1], 'w')
        fp.write(json.dumps(check_api, indent=1))
        fp.close()

        self.status.PrintDoneMessage()

    def apiTodict(self, text_api):
        text_itemlist = text_api.splitlines(True)
        dict_api = dict()
        for item in text_itemlist:
          key = item.strip().split('=', 1)[0]
          value = item.strip().split('=', 1)[1]
          dict_api[key] = value
        return dict_api

def main(arguments=None):
  if arguments is None:
    arguments = sys.argv
  options, target_ip = parseOptions(arguments)
  test = CheckDefaultValues(target_ip, options.login_id, options.login_password)
  test.Start(options.default_file)

if __name__ == "__main__":
    exitCode = main()
    sys.exit(exitCode)
