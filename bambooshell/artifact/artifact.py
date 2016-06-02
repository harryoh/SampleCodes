import boto.sdb
import os
import antglob
import sys
import threading
import Queue

__version_info__ = (0, 0, 1)
__version__ = '.'.join(unicode(item) for item in __version_info__)

_Usage = "%prog <options> Files(Ant-Pattern)"
_Description = "Upload the artifacts searched to S3"

aws_contain_name = 'bamboo-artifact'
bamboo_hostname = os.getenv('bamboo_hostname')
if bamboo_hostname is not None:
  aws_contain_name += '-' + bamboo_hostname

region = "us-east-1"

success = False

def parseOptions(arguments):
    assert arguments is not None
    from optparse import OptionParser

    parser = OptionParser(usage=_Usage, description=_Description,
                          version="%prog " + __version__)
    parser.add_option("-n", "--name", dest="artifactname",
                      help="Specify Name of the artifact(s)",
                      metavar="AntFilePattern")
    parser.add_option("-l", "--location", dest="location", default=".",
                      help="Specify the directory (including path) where \
                      the artifacts are")
    parser.add_option("--aws_access_key_id", dest='aws_access_key_id',
                      help="Amazon Access Key ID")
    parser.add_option("--aws_secret_access_key", dest='aws_secret_access_key',
                      help="Amazon Secret Access Key")
    (options, others) = parser.parse_args(arguments[1:])

    if options.aws_access_key_id is None:
      bamboo_aws_access_key_id = os.getenv('bamboo_aws_access_key_id_password')
      if bamboo_aws_access_key_id is not None:
        options.aws_access_key_id = bamboo_aws_access_key_id
      else:
        parser.error('option --aws_access_key_id must be set')
    if options.aws_secret_access_key is None:
      bamboo_aws_secret_access_key = os.getenv(
                                      'bamboo_aws_secret_access_key_password')
      if bamboo_aws_secret_access_key is not None:
        options.aws_secret_access_key = bamboo_aws_secret_access_key
      else:
        parser.error('option --aws_secret_access_key must be set')
    if options.artifactname is None:
        parser.error("option --name must be set")
    if len(others) == 0:
        parser.error("File name or ant file pattern the artifact(s) \
                      you want to copy must be set")
    pattern = others[0]
    return (options, pattern)

class BotoError(Exception): pass
class S3Error(BotoError): pass
class SimpleDBError(BotoError): pass

class SimpleDB:
  def __init__(self, aws_access_key_id, aws_secret_access_key):
    assert aws_access_key_id is not None
    assert aws_secret_access_key is not None
    self.aws_access_key_id = aws_access_key_id
    self.aws_secret_access_key = aws_secret_access_key
    self.conn = None
    self.dom = None
    self.add_list = []

  def Connect(self):
    try:
      self.conn = boto.sdb.connect_to_region(
                    'us-east-1',
                    aws_access_key_id=self.aws_access_key_id,
                    aws_secret_access_key=self.aws_secret_access_key)
    except Exception, e:
      raise SimpleDBError(e)

  def _has_domain(self, domain_name):
    return domain_name in [d.name for d in self.conn.get_all_domains()]

  def GetDomain(self, domain_name):
    assert self.conn is not None
    assert domain_name is not None
    try:
      if self._has_domain(domain_name) is True:
        self.dom = self.conn.get_domain(domain_name)
      else:
        self.dom = self.conn.create_domain(domain_name)
      return self.dom
    except Exception, e:
      raise SimpleDBError(e)

  def DeleteDomain(self, domain_name):
    assert self.conn is not None
    assert domain_name is not None
    try:
      self.conn.delete_domain(domain_name)
    except Exception, e:
      raise SimpleDBError(e)

  def AddItem(self, item_name, attributes):
    assert item_name is not None
    assert self.dom is not None
    try:
      self.dom.put_attributes(item_name, attributes)
      self.add_list.append(item_name)
    except Exception, e:
      raise SimpleDBError(e)

  def GetItem(self, item_name):
    assert item_name is not None
    assert self.dom is not None
    try:
      return self.dom.get_item(item_name)
    except Exception, e:
      raise SimpleDBError(e)

  def DeleteItem(self, item_name):
    assert item_name is not None
    assert self.dom is not None
    try:
      item = self.GetItem(item_name)
      if item is None:
        return -1
      self.dom.delete_item(item)
      return 0
    except Exception, e:
      raise SimpleDBError(e)

  def DeleteAddedItems(self):
    assert self.dom is not None
    try:
      for item in self.add_list:
        self.DeleteItem(item)
    except Exception, e:
      raise SimpleDBError(e)

class S3:
  def __init__(self, aws_access_key_id, aws_secret_access_key):
    assert aws_access_key_id is not None
    assert aws_secret_access_key is not None
    self.aws_access_key_id = aws_access_key_id
    self.aws_secret_access_key = aws_secret_access_key
    self.conn = None

  def Connect(self):
    import boto.s3.connection
    try:
      self.conn = boto.connect_s3(
        aws_access_key_id = self.aws_access_key_id,
        aws_secret_access_key = self.aws_secret_access_key,
        calling_format = boto.s3.connection.OrdinaryCallingFormat(),)
    except Exception, e:
      raise S3Error(e)

  def GetBucketList(self):
    assert self.conn is not None
    result = []
    try:
      for bucket in self.conn.get_all_buckets():
        result.append(bucket)
    except Exception, e:
      raise S3Error(e)
    return result

  def GetBucket(self, aws_bucket_name):
    assert aws_bucket_name is not None
    bucket = self.conn.lookup(aws_bucket_name)
    if bucket is None:
      try:
        bucket = self.conn.create_bucket(aws_bucket_name)
      except Exception, e:
        raise S3Error(e)
    self.bucket = bucket
    return bucket

  def SetStorePath(self, store_path):
    self.store_path = store_path

  def Upload(self, filename):
    assert filename is not None
    assert self.bucket
    assert self.store_path
    from boto.s3.key import Key

    try:
      k = Key(self.bucket)
      k.key = self.store_path + '/' + os.path.basename(filename)
      k.set_contents_from_filename(filename)
      return filename
    except Exception, e:
      raise S3Error(e)

  def DeleteOjbect(self, prefix):
    assert self.bucket
    try:
      for key in self.bucket.list(prefix=prefix):
        key.delete()
    except Exception, e:
      raise S3Error(e)

class ThUpload(threading.Thread):
  sdb = None
  def __init__(self, queue, S3, BambooInfo):
    threading.Thread.__init__(self)
    self.s3 = S3
    self.bamboo = BambooInfo
    self.queue = queue
    try:
      if self.sdb is None:
        self.sdb = SimpleDB(S3.aws_access_key_id, S3.aws_secret_access_key)
        self.sdb.Connect()
        self.sdb.GetDomain(aws_contain_name)
    except Exception, e:
      raise SimpleDBError(e)

  def run(self):
    import datetime
    global success
    while True:
      filename = self.queue.get()
      try:
        self.s3.Upload(filename)
        self.sdb.AddItem(self.bamboo.bamboo_buildResultKey + '-' +
                         self.bamboo.artifactname + '-' +
                         os.path.basename(filename), {
          'artifact_name': self.bamboo.artifactname,
          'filename': os.path.basename(filename),
          'filesize': os.path.getsize(filename),
          'buildProject': self.bamboo.buildProject,
          'buildPlan': self.bamboo.buildPlan,
          'buildJob': self.bamboo.buildJob,
          'buildNumber': self.bamboo.buildNumber,
          'buildResultsUrl': self.bamboo.bamboo_buildResultsUrl,
          's3ResultUrl': self.s3.store_path,
          'nasResultUrl': '',
          'datetime': datetime.datetime.utcnow().isoformat()}
        )
      except Exception, e:
        print >> sys.stderr, e
        success = False
        self.sdb.DeleteAddedItems()
      else:
        success = True
      finally:
        self.queue.task_done()

class BambooInfo:
  artifactname = None
  def __init__(self):
    self.bamboo_buildResultKey = os.getenv('bamboo_buildResultKey')
    self.bamboo_buildResultsUrl = os.getenv('bamboo_buildResultsUrl')
  
    assert self.bamboo_buildResultKey is not None
    assert self.bamboo_buildResultsUrl is not None
  
    bamboo_list = self.bamboo_buildResultKey.split('-')
  
    self.buildProject = bamboo_list[0]
    self.buildPlan = bamboo_list[1]
    self.buildJob = bamboo_list[2]
    self.buildNumber = bamboo_list[3]

def main(arguments=None):
  if arguments is None:
      arguments = sys.argv
  options, pattern = parseOptions(arguments)
  FileSet = antglob.AntPatternSet()
  FileSet.include(pattern)

  bamboo = BambooInfo()
  bamboo.artifactname = options.artifactname

  store_path = bamboo.buildProject
  store_path += '/' + bamboo.buildPlan
  store_path += '/' + bamboo.buildNumber
  store_path += '/' + options.artifactname

  filelist = FileSet.find(options.location)
  fp = file('result.txt', 'a+')
  fp.write('Artifactname: ' + options.artifactname + '\n')
  fp.write('S3: ' + store_path + '\n')

  print '[Uploading to S3]'
  print ' containname:' + aws_contain_name
  print ' artifactname: ' + options.artifactname
  print ' location: ' + options.location
  print ' pattern: ' + pattern
  try:
    s3 = S3(options.aws_access_key_id, options.aws_secret_access_key)
    s3.Connect()
    s3.GetBucket(aws_contain_name)
    s3.SetStorePath(store_path)
    queue = Queue.Queue()

    for i in range(len(filelist)):
      i
      t = ThUpload(queue, s3, bamboo)
      t.setDaemon(True)
      t.start()

    for filename in filelist:
      if options.location is not None:
        filename = options.location + '/' + filename
      queue.put(filename)
      fp.write(' - ' + filename + '\n')
      print '- ' + filename
      
    from time import sleep 
    sleep(0.1)
    queue.join()

    if success is False:
      s3.DeleteOjbect(
          bamboo.buildProject + '/' +
          bamboo.buildPlan + '/' +
          bamboo.buildNumber)

      fp.write('*** Error\n\n')
      fp.close()
      print "Error"
      os._exit(-1)
    else:
      fp.write('Done\n\n')
      fp.close()
      print "Done"

  except BotoError as e:
    print e

  return 0

if __name__ == "__main__":
  exitCode = main()
  sys.exit(exitCode)
