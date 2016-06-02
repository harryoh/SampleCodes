import os, sys
import json
import time
from ipx_status import Status

__version_info__ = (0, 0, 1)
__version__ = '.'.join(unicode(item) for item in __version_info__)

_Usage = "%prog --login_id <id for targetip> --login_password <password for targetip> <TargetIP>"
_Description = "Check Status of a camera"

class ResourceError(Exception): pass
class EncoderError(Exception): pass

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

class CheckStatus:
    def __init__(self, target_ip, login_id, login_password):
        assert target_ip is not None
        self.target_ip = target_ip
        self.status = Status(target_ip, login_id, login_password)
        self.status.PrintInformation(self.status.GetStatus('brand'))
        print

    def CheckResource(self):
        min_idle = 30
        max_idle = 95
        test_count = 3
        total_idle = 0
        avg_idle = 0

        for count in range(0, test_count):
            if count is not 0:
                time.sleep(3)
            self.status.PrintStartMessage(' Start Check Resource (%d Time)'
                % (count + 1))
            resource = self.status.GetStatus('resource')['resource-state']['resource']
            if not os.path.exists('result'): os.makedirs('result')
            fp = file('result/resource-%s_%d.txt'
                % (self.target_ip, count + 1), 'w')
            fp.write(json.dumps(resource, indent=1))
            fp.close()
            for i in range(0, len(resource)):
                if resource[i]['@name'] == 'cpu':
                    print "[CPU Information]"
                    for k in range(0, len(resource[i]['item'])):
                        print str(resource[i]['item'][k]['@name']) + " : "\
                              + str(resource[i]['item'][k]['#text'])
                    for k in range(0, len(resource[i]['item'])):
                        if resource[i]['item'][k]['@name'] == 'idle':
                            total_idle += float(resource[i]['item'][k]['#text'])
        avg_idle = total_idle / test_count
        self.status.PrintStartMessage('Avarage Idle: %r' % avg_idle)
        if avg_idle < min_idle:
            raise ResourceError('CPU Idle is too low(idle:%r).' % avg_idle)
        elif avg_idle > max_idle:
            raise ResourceError('CPU Idle is too high(idle:%r).' % avg_idle)

        self.status.PrintDoneMessage()
        return 0

    def CheckEncoder(self):
        min_bitrate = 1000
        test_count = 3
        total_bitrate = 0
        avg_bitrage = 0

        for count in range(0, test_count):
            if count is not 0:
                time.sleep(3)
            self.status.PrintStartMessage(' Start Check Encoder (%d Time)'
                % (count + 1))
            encoder = self.status.GetStatus('encode')['encode']['stream']
            if not os.path.exists('result'): os.makedirs('result')
            fp = file('result/encoder-%s_%d.txt'
                % (self.target_ip, count + 1), 'w')
            fp.write(json.dumps(encoder, indent=1))
            fp.close()
            print '[Encoder Information]'
            print json.dumps(encoder, indent=1)
            for i in range(0, len(encoder)):
                total_bitrate += int(encoder[i]['bitrate'])
        avg_bitrate = total_bitrate / test_count
        self.status.PrintStartMessage('Avarage Bitrate: %r' % avg_bitrate)
        if avg_bitrate < min_bitrate:
            raise EncoderError('Bitrate is too low (bitrate: %r)' % avg_bitrate)
        self.status.PrintDoneMessage()
        return 0

def main(arguments=None):
    if arguments is None:
        arguments = sys.argv
    options, target_ip = parseOptions(arguments)
    test = CheckStatus(target_ip, options.login_id, options.login_password)
    test.CheckResource()
    test.CheckEncoder()
    return 0

if __name__ == "__main__":
    exitCode = main()
    sys.exit(exitCode)
