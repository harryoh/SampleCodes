import urllib2
import socket
import time
import xmltodict

class Timeout(Exception): pass

class Status:
    def __init__(self, target_ip, login_id, login_password):
        assert target_ip is not None
        if login_id is None: login_id = 'root'
        if login_password is None: login_password = 'pass'

        self.target_ip = target_ip
        self.login_id = login_id
        self.login_password = login_password
        self.statusfiles = {
            'brand': '/brand.xml',
            'encode': '/status/encode.xml',
            'package': '/cust/version.xml',
            'resource': '/status/resource-state.xml'}

    def SetTargetIP(self, target_ip):
        assert target_ip is not None
        self.target_ip = target_ip

    def PrintStartMessage(self, message):
        print '*' * 40
        print message
        print '*' * 40

    def PrintDoneMessage(self):
        print '-' * 15 + ' Done ' + '-' * 15
        print

    def PrintInformation(self, brand_obj):
        print "[Camera]"
        print "Target IP: " + self.target_ip
        print "Serial: " + brand_obj['brand']['model']['serial']
        print 'Firmware: ' + brand_obj['brand']['version']['firmware']
        if 'revision' in brand_obj['brand']['version']:
            print 'Revision: ' + brand_obj['brand']['version']['revision']

    def GetStatus(self, item, timeout=5, timeout_once=5):
        url = 'http://' + self.target_ip + self.statusfiles[item]
        if (timeout_once > timeout):
            timeout_once = timeout
        count = timeout/timeout_once
        if (timeout%timeout_once != 0):
            count+=1
        for i in range(0, count):
            i
            if (timeout < timeout_once):
                timeout_once = timeout
            try:
                status_obj = xmltodict.parse(urllib2.urlopen(url, timeout=timeout_once).read())
                return status_obj
            except urllib2.URLError, e:
                if isinstance(e.reason, socket.timeout):
                    timeout -= timeout_once
                    continue
                if isinstance(e.reason, socket.error):
                    timeout -= timeout_once
                    time.sleep(timeout_once)
                    continue
        raise Timeout(e)
