Box User Setting
=================

가상머신의 설정은 기본적으로 제공한다. (vagrant/develop/Vagrantfile)  
하지만 사용자별로 Memory, CPU, Network등 별도의 설정을 필요로 할때가 있다.

사용자가 임의로 Guest OS의 설정을 변경하는 방법에 대해서 설명한다.


Virtual Box를 이용
-----------------

Virtual Box에서 직접 설정을 변경하여도 그대로 사용이 가능하다.  
하지만 Guest OS를 삭제하고 다시 설치할 경우에는 또 다시 설정을 변경해야한다.  
`UserVagrantfile`을 작성하게 되면 Guest OS를 삭제하고 새로 받는 다고 해도 설정을 유지할 수가 있다.


UserVagrantfile 작성
--------------------

Guest OS의 사양은 UserVagrantfile에 기록하여 원하는 데로 사용할 수가 있다.  
`UserVagrantfile.sample`을 참고하여 작성 할 수가 있다.

    # 먼저 Repository의 Root 디렉토리로 이동한다.
    $ cd vagrant/develop
    $ ls
    UserVagrantfile.sample  Vagrantfile
    $ cp UserVagrantfile.sample UserVagrant

### UserVagrantfile

See [Vagrant Documentation](http://docs.vagrantup.com/v2/) for detail.

* config.vm.network :forwarded_port

    외부에서 Guest OS로 접근할 수 있도록 Port Forwarding 설정한다.  

* config.vm.synced_folder

        # Guest OS와 Host OS와의 공유를 설정한다.  
        `config.vm.synced_folder "../../../", "/work"`이 기본적으로 설정되어 있어서 
        Host OS의 Repository의 상위 폴더와 Guest OS의 /work 디렉토리가 공유되어있다.  

    Guest OS에서 /home/vagrant/work와 /work는 서로 링크되어있다.

* Virtual Machine Customization

        config.vm.provider "virtualbox" do |v|
            v.memory = 1024
            v.cpus = 1
        end

* config.vm.network

    Ip의 값을 사용하지 않으면 DHCCP서버를 이용한다.

    - private_network  : `config.vm.network :private_network, ip: "192.168.33.10`

    - public_network :  `config.vm.network :public_network`



