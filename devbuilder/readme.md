DevBuilder Project
================

* 본 문서의 오류나 개선할 여지를 발견하면 `harry.oh@udptechnology.com`으로 연락바랍니다.

* 용어
    * `HOST`: 사용자의 PC
    * `Guest OS`: Virtual Machine

Table of Contents
-----------------

<!-- MarkdownTOC depth=3 -->

- What's the DevBuilder?
- Installing
    - Install Required Pckages
- Vagrant
    - Start Guest OS
    - Show status the Guest OS
    - Shutdown the Guest OS
    - Destroy the Guest OS
- Shared Folder
- Test

<!-- /MarkdownTOC -->

---
What's the DevBuilder?
--------------------

`DevBuilder`는 자동화된 개발 서버의 구축 프로젝트이다(Virtual Infrastructure Automation)이다.

Embedded 장비를 개발하기 위해서 Cross-Compile을 할 수 있는 환경이 필수적이다.

개발자들이 늘어나고 제품이 많아지면서 하나의 서버에서 모든 개발자들이 모든 제품을 개발하는 것은 
점점 어려워지고 비효율적이 되가고 있다.  
개발자는 자신의 개발서버를 구축함으로 좀 더 자유롭게 개발할 수가 있는데 몇가지 문제가 수반된다.

개발서버를 구축하기 위해서는 서버 설치의 시간이 필요하게 되며 기초적인 지식이 부족할 경우에는 더 많은 
시간이 필요하거나 포기하게 된다. 또한 새로운 제품을 개발하거나 개발서버의 환경이 변경되어야 할 때 모든 
개발자들이 작업해야하는 번거로움도 발생한다.  
게다가 각자가 서로 다른 환경의 개발서버에서 작업하게 되면 서로 다른 결과가 나와 협업에 걸림돌이 되기도 
한다.

`DevBuilder`는 개발자들이 별도의 개발서버를 구축하여 사용할때에 생기는 불필요한 시간과 노력을 최소화하고 
서로 다른 환경에서 생기는 문제를 발생하지 않도록 한다.  
그리고 개발의 노하우를 공유하여 개발의 속도에 도움이 되고자 하며 장기적으로는 
`Ip Camera Simulator`를 구현하기 위한 기반 프로젝트이다.

`DevBuilder`를 이용하여 몇개의 명령으로 단 몇분만에 개발환경을 구축하여 시간을 단축할 수 있으며 
Network File System(SMB, NFS등)을 이용하여 개발하면서 네트워크의 품질에 따라서 개발이 지연되는 
현상을 없앨 수가 있다.

---
Installing
-----------

##### Required Packages:

* Virtual Box: [http://www.virtualbox.org][virtualbox]
* Vagrant: [http://www.vagrantup.com][vagrant]

### Install Required Pckages

#### Install Virtual Box.  

Download from [https://www.virtualbox.org/wiki/Linux_Downloads][virtualbox_download]  
And install it.

![Warning][icon_atlassian_warning] Linux에서 Virtual Box를 실행할때 Kernel Version 에러가 발생하면
 
    sudo /etc/init.d/vboxdrv setup

`vboxdrv setup` 실행시 오류가 난다면 ftp://ftp.kernel.org/pub/linux/kernel/ 에서 
`uanme -a`로 확인한 가장 근접한 버젼을 다운 받고 Kernel Source의 Makefile에서 동일한 버젼으로 
수정후 다시 실행하면 동작한다.

#### Install Vagrant

Download from [http://www.vagrantup.com/downloads.html][vagrant_download]  
And install it.

#### Clone DevBuilder repository

    $ git clone https://bitbucket.org/udptech/devbuilder.git

or use `GUI Git Client`(such as source tree, tower).  
See the names of the Guest OS. `develop` is one of the Guest OS.

    $ ls -l devenest/vagrant
    total 16
    -rw-r--r--  1 harry  staff   4.5K Apr  2 14:54 Vagrantfile.default
    drwxr-xr-x  5 harry  staff   170B Apr  4 16:51 develop/

---
Vagrant
-------

### Start Guest OS

Go to a Guest OS in DevBuilder repository and run `vagrant up`

    $ cd ~/work/devbuilder/vagrant/develop
    $ vagrant up


Messages:

```
Bringing machine 'default' up with 'virtualbox' provider...
[default] Box 'develop' was not found. Fetching box from specified URL for
the provider 'virtualbox'. Note that if the URL does not have
a box for this provider, you should interrupt Vagrant now and add
the box yourself. Otherwise Vagrant will attempt to download the
full box prior to discovering this error.
Downloading box from URL: http://192.168.2.152/public/vagrant-boxes/Ubuntu-Server.box
Extracting box...te: 11.1M/s, Estimated time remaining: 0:00:01)
Successfully added box 'develop' with provider 'virtualbox'!
[default] Importing base box 'develop'...
[default] Matching MAC address for NAT networking...
[default] Setting the name of the VM...
[default] Clearing any previously set forwarded ports...
[default] Clearing any previously set network interfaces...
[default] Preparing network interfaces based on configuration...
[default] Forwarding ports...
[default] -- 22 => 2222 (adapter 1)
[default] -- 21 => 50021 (adapter 1)
[default] -- 22 => 50022 (adapter 1)
[default] Booting VM...
[default] Waiting for machine to boot. This may take a few minutes...
[default] Machine booted and ready!
[default] Mounting shared folders...
[default] -- /work
[default] -- /vagrant
[default] -- /tmp/vagrant-chef-1/chef-solo-3/roles
[default] -- /tmp/vagrant-chef-1/chef-solo-2/cookbooks
[default] -- /tmp/vagrant-chef-1/chef-solo-1/cookbooks
[default] Running provisioner: chef_solo...
Generating chef JSON and uploading...
Running chef-solo...
stdin: is not a tty
[2014-04-04T16:53:58+09:00] INFO: Forking chef instance to converge...
[2014-04-04T16:53:58+09:00] INFO: *** Chef 11.10.4 ***
[2014-04-04T16:53:58+09:00] INFO: Chef-client pid: 2480
[2014-04-04T16:53:59+09:00] INFO: Setting the run_list to ["role[develop]"] from JSON
[2014-04-04T16:53:59+09:00] INFO: Run List is [role[develop]]
[2014-04-04T16:53:59+09:00] INFO: Run List expands to [cross-tools::codesourcery, cross-tools::ti]
[2014-04-04T16:53:59+09:00] INFO: Starting Chef Run for ubuntu-server.box
[2014-04-04T16:53:59+09:00] INFO: Running start handlers
[2014-04-04T16:53:59+09:00] INFO: Start handlers complete.
[2014-04-04T16:54:39+09:00] INFO: bash[install codesourcery] ran successfully
[2014-04-04T16:57:01+09:00] INFO: bash[install ti-toolchain] ran successfully
[2014-04-04T16:57:01+09:00] INFO: Chef Run complete in 182.334579633 seconds
[2014-04-04T16:57:01+09:00] INFO: Running report handlers
[2014-04-04T16:57:01+09:00] INFO: Report handlers complete
```

Now, you can connect to the GUEST OS is named `develop`.

    $ vagrant ssh

or

    $ ssh vagrant@localhost -p 2222
    password:

Case a window, use putty or other ssh client.

    Host: localhost(127.0.0.1)
    Port: 2222
    Username: vagrant
    Password: vagrant

### Show status the Guest OS

    $ vagrant status
    Current machine states:

    default                   running (virtualbox)

    The VM is running. To stop this VM, you can run `vagrant halt` to
    shut it down forcefully, or you can run `vagrant suspend` to simply
    suspend the virtual machine. In either case, to restart it again,
    simply run `vagrant up`.

### Shutdown the Guest OS

Shutdown을 하더라도 Guest OS의 내용은 삭제되지 않는다.  
`vagrant up`을 다시하면 원래 그대로 살아난다.

    $ vagrant halt
    [default] Attempting graceful shutdown of VM...

    $ vagrant status
    Current machine states:

    default                   poweroff (virtualbox)

    The VM is powered off. To restart the VM, simply run `vagrant up`

### Destroy the Guest OS

    $ vagrant destroy
    Are you sure you want to destroy the 'default' VM? [y/N] y
    [default] Destroying VM and associated drives...
    [default] Running cleanup tasks for 'chef_solo' provisioner...

    $ vagrant box list
    develop (virtualbox)

    $ vagrant box remove develop
    Removing box 'develop' with provider 'virtualbox'...

---
Shared Folder
--------------

`Host` 와 `Guest`간에 Shared Folder를 만들어서 Host에서 더 쾌적하게 작업할 수가 있도록 한다.  
실제 데이터는 Host에 존재하며 Guest에서 사용한다. 때문에 Shared Folder에서 작업했다면 
Guest가 지워지더라도 데이터는 사라지지 않는다.

* Host folder: The `parent folder` of DevBuilder repository.
* Guest OS folder: `/work` or `/home/vagrant/work` (symbolic link)

---
Test
----

1. Host에서 Guest OS를 새롭게 만들어 구동시킨다.
2. Host에서 새로운 Repository를 생성한다.
3. Guest OS에서 Host에서 생성한 Repository의 Log를 확인한다.

Command:

    ##### Host
    $ mkdir ~/work
    $ cd ~/work
    $ git clone https://bitbucket.org/udptech/devbuilder.git
    $ cd devbuilder/vagrant/develop
    $ vagrant up
    Bringing machine 'default' up with 'virtualbox' provider...
    [default] Box 'develop' was not found. Fetching box from specified URL for
    the provider 'virtualbox'. Note that if the URL does not have
    ...
    ...
    [2014-04-04T16:57:01+09:00] INFO: Running report handlers
    [2014-04-04T16:57:01+09:00] INFO: Report handlers complete

    $ vagrant ssh

    ##### Guest OS
    $ cd work
    $ ls
    devbuilder
    $ exit

    ##### Host
    $ cd ~/work
    $ mkdir test.git
    $ cd test.git
    $ git init
    Initialized empty Git repository in /Users/harry/work/test.git/.git/

    $ touch first
    $ git add first
    $ git commit -m "First Commit"
    [master (root-commit) acd52c7] First Commit
     1 file changed, 0 insertions(+), 0 deletions(-)
     create mode 100644 first

    $ git log
    commit acd52c7e4273175ff3816d76823f97264bef1890
    Author: Harry Oh <harry.oh@udptechnology.com>
    Date:   Fri Apr 4 17:43:36 2014 +0900

        First Commit

    $ ssh vagrant@localhost -p 2222
    password:

    ##### Guest OS
    $ cd work/test.git
    $ git log
    commit acd52c7e4273175ff3816d76823f97264bef1890
    Author: Harry Oh <harry.oh@udptechnology.com>
    Date:   Fri Apr 4 17:43:36 2014 +0900

        First Commit

[virtualbox]: http://www.virtualbox.org
[vagrant]: http://vagrantup.com
[virtualbox_download]: https://www.virtualbox.org/wiki/Linux_Downloads
[vagrant_download]: http://www.vagrantup.com/downloads.html

[icon_atlassian_smile]: http://goo.gl/5AXCw8
[icon_atlassian_sad]: http://goo.gl/RLDQmr
[icon_atlassian_tongue]: http://goo.gl/Nc8Lp9
[icon_atlassian_biggrin]: http://goo.gl/JtBehQ
[icon_atlassian_wink]: http://goo.gl/49JMG7
[icon_atlassian_thumbsup]: http://goo.gl/kO7sli
[icon_atlassian_thumbsdown]: http://goo.gl/pwDjkm
[icon_atlassian_info]: http://goo.gl/MrKVZD
[icon_atlassian_tick]: http://goo.gl/d82dff
[icon_atlassian_error]: http://goo.gl/ydZXmJ
[icon_atlassian_warning]: http://goo.gl/cYPvUQ
[icon_atlassian_plus]: http://goo.gl/jRihe1
[icon_atlassian_minus]: http://goo.gl/M831SJ
[icon_atlassian_question]: http://goo.gl/9YdbgT
[icon_atlassian_lighton]: http://goo.gl/ZaQNh7
[icon_atlassian_lightoff]: http://goo.gl/Pyjt4h
[icon_atlassian_yellow_star]: http://goo.gl/fDsg7F
[icon_atlassian_red_star]: http://goo.gl/nEsp7J
[icon_atlassian_green_star]: http://goo.gl/f7X959
[icon_atlassian_blue_star]: http://goo.gl/kEIRvK
