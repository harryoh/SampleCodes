How to create vagrant custom box
================================

<!-- MarkdownTOC depth=3 -->

- Install Packages
    - Virtual Box
    - Vagrant
- Create a new virtual box image
- Setup the Guest OS (Virtual machine)
- Install package to Guest OS (virtual machine)
- Install virtual box guest additions
- Setup GIT configuration
- Packaging Vagrant Box

<!-- /MarkdownTOC -->

Install Packages
----------------

### Virtual Box

Download from [https://www.virtualbox.org/wiki/Linux_Downloads][virtualbox_download]  
And install it.

### Vagrant

Download from [http://www.vagrantup.com/downloads.html][vagrant_download]  
And install it.

Create a new virtual box image
-------------------------------

Download ubuntu iso file from [http://www.ubuntu.com][ubuntu].
And install it to virtual machine.

* Hostname: **vagrant-[os-name]**, e.g. vagrant-debian-lenny
* Domain: **vagrantup.com** (causes delays in resolving own FQDN, as in dnsdomainname or hostname --fqdn commands)
* Root Password: **pass**
* New account login: **vagrant**
* New account password: **vagrant**

Setup the Guest OS (Virtual machine)
-------------------------------------

Add `vagrant` user and `admin` group. ▼

    sudo groupadd admin
    sudo usermod -G admin vagrant

Insert following lines to `/etc/sudoers`. ▼

    Defaults env_keep="SSH_AUTH_SOCK"
    %admin ALL=NOPASSWD: ALL

After that, `/etc/sudoers` should look like this. ▼

```
#
# This file MUST be edited with the 'visudo' command as root.
#
# Please consider adding local content in /etc/sudoers.d/ instead of
# directly modifying this file.
#
# See the man page for details on how to write a sudoers file.
#
Defaults   env_reset
Defaults   mail_badpass
Defaults   secure_path="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
Defaults   env_keep="SSH_AUTH_SOCK"

# Host alias specification

# User alias specification

# Cmnd alias specification

# User privilege specification
root   ALL=(ALL:ALL) ALL

# Members of the admin group may gain root privileges
%admin ALL=(ALL) ALL
%admin ALL=NOPASSWD: ALL

# Allow members of group sudo to execute any command
%sudo  ALL=(ALL:ALL) ALL

# See sudoers(5) for more information on "#include" directives:

#includedir /etc/sudoers.d
```

Install vagrant's public keys. ▼

    mkdir ~/.ssh/
    chmod 0755 ~/.ssh
    cd ~/.ssh
    wget http://github.com/mitchellh/vagrant/raw/master/keys/vagrant
    wget http://github.com/mitchellh/vagrant/raw/master/keys/vagrant.pub
    mv vagrant.pub authorized_keys
    chmod 0644 authorized_keys


Install package to Guest OS (virtual machine)
----------------------------------------------

Change **kr.archive.ubuntu.com** to **ftp.daum.net** in sources.list. ▼

    sed -i 's/kr\.archive\.ubuntu\.com/ftp\.daum\.net/g' /etc/apt/sources.list

Upgrade APT repository newly. ▼

    apt-get update  
    apt-get -y upgrade  

And install Develop packages. ▼

    sudo apt-get install -y gcc g++ libxml2-dev libxslt1-dev libacl1-dev liblzo2-dev uuid-dev make graphviz-dev fakeroot sqlite3 libsqlite3-dev xmlstarlet mtd-utils u-boot-tools cppcheck libcppunit-dev ack-grep libfreetype6-dev python-bitarray doxygen texlive ccache git vim

> Make **vim.basic** is to be your default editor.

After that, install Chef. ▼

    curl -L http://www.opscode.com/chef/install.sh | bash

Install virtual box guest additions
-------------------------------------

Install dkms and reboot. ▼

    sudo apt-get -y install linux-headers-$(uname -r)build-essential dkms
    sudo reboot

Click on your VirtualBox window and select “Devices” and “Install Guest Additions”. The option is in the VM window, and not in the VirtualBox control panel. This will attach a virtual CD to your device, which you can mount and install the software from. ▼

    sudo apt-get -y install linux-headers-$(uname -r) build-essential
    mkdir /media/cdrom
    mount /dev/cdrom /media/cdrom
    sudo sh /media/cdrom/VBoxLinuxAdditions.run

Setup GIT configuration
------------------------

**Auto Completion**

See [https://udptech.jira.com/wiki/pages/viewpage.action?spaceKey=RND&title=GIT+auto+complete](https://udptech.jira.com/wiki/pages/viewpage.action?spaceKey=RND&title=GIT+auto+complete)

* Download git-completion.sh at [GIT auto complet](https://udptech.jira.com/wiki/pages/viewpage.action?spaceKey=RND&title=GIT+auto+complete)  
* Copy *git-completion.sh* downloaded to **/etc/profile.d/**

**Useful Config**

See [https://udptech.jira.com/wiki/display/RND/GIT+Useful+Config](https://udptech.jira.com/wiki/display/RND/GIT+Useful+Config)

    cat > /etc/profile.d/git_userful_config.sh

    #!/bin/bash
    git config --global alias.co checkout
    git config --global alias.br branch
    git config --global alias.ci commit
    git config --global alias.st status
    git config --global core.editor vim
    git config --global color.diff auto
    git config --global color.status auto
    git config --global color.ui true
    git config --global alias.history "log --pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr) %C(bold blue)<%an>%Creset' --abbrev-commit --all --graph --decorate --color"
    git config --global credential.helper "cache --timeout=3600"

`[Ctrl + D]`

**Branch name on prompt**

    cat >> ~/.profile

    function parse_git_branch () {
      local is_git=$(git rev-parse --is-inside-work-tree 2>&-)
      
      if [ "$is_git" = "true" ]; then
        branch_ref=$(git symbolic-ref HEAD 2>&-)
        if [ -z "$branch_ref" ]; then
          branch="$(git branch --no-color | sed -e '/^[^*]/d' -e "s/* (//" -e "s/)//"|cut -d' ' -f3 2>&-)"
          branch_prefix="detached"
          for tag in $(git tag)
            do
              if [ "$branch" = "$tag" ]; then
                branch_prefix="tag"
              fi
            done
            branch="$branch_prefix:$branch"
        else
          branch=${branch_ref#refs/heads/}
        fi
        local status=$(git diff --quiet 2>&-)
        [ -z "$status" ] || branch="$branch*"
        [ "$branch" ] && echo "($branch)"
      fi
    }
    
    YELLOW="\[\033[0;33m\]"
    NO_COLOUR="\[\033[0m\]"

    PS1="\u@\h:\w$YELLOW\$(parse_git_branch)$NO_COLOUR# "

`[Ctrl+D]`

**Clean machine**

Empty the cache.  ▼

    apt-get clean   

Remove user history. ▼

    rm -f ~/.bash_history  

**Shutdown**

    shutdown -h now

Packaging Vagrant Box
----------------------

    mkdir ~/VirtualBox\ VMs/Vargrant-Boxes
    cd ~/VirtualBox\ VMs/Vargrant-Boxes
    vagrant package --base Ubuntu-Server --output Ubuntu-Server.box

And testing the VM. ▼

    vagrant box add base ~/VirtualBox\ VMs/Vagrant-Boxes/Ubuntu-Server.box
    vagrant init base
    vagrant up
    vagrant ssh-config --host localhost >> ~/.ssh/config
    vagrant halt

After test, Upload the box to `shared space`.  
Like at `ftp://192.168.2.152/public/vagrant-boxes/`

[virtualbox]: http://www.virtualbox.org
[vagrant]: http://vagrantup.com
[ubuntu]: http://www.ubuntu.com
[virtualbox_download]: https://www.virtualbox.org/wiki/Linux_Downloads
[vagrant_download]: http://www.vagrantup.com/downloads.html