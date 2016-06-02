#
# Cookbook Name:: cross-tools
# Recipe:: codesourcery
#
# Copyright 2014, YOUR_COMPANY_NAME
#
# All rights reserved - Do Not Redistribute
#

bash "install codesourcery" do
    user 'vagrant'
    group 'admin'
    cwd '/home/vagrant'
    environment 'TOOLCHAIN' => 'arm-2012.09-64-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2'
    code <<-EOC
        [ -e "$TOOLCHAIN" ] && rm -f $TOOLCHAIN
        if curl -O http://192.168.2.152/public/toolchain/$TOOLCHAIN; then
            sudo mkdir -p /opt/codesourcery
            sudo tar -jxf $TOOLCHAIN -C /opt/codesourcery
            rm -f $TOOLCHAIN
            echo -n "export PATH=$" | cat > codesourcery.sh
            echo "PATH:/opt/codesourcery/arm-2012.09/bin" | cat >> codesourcery.sh
            sudo mv codesourcery.sh /etc/profile.d/
        fi
    EOC
    creates "/opt/codesourcery"
end