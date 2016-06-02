#
# Cookbook Name:: cross-tools
# Recipe:: ti
#
# Copyright 2014, YOUR_COMPANY_NAME
#
# All rights reserved - Do Not Redistribute
#

bash "install ti-toolchain" do
    user 'vagrant'
    group 'admin'
    cwd '/home/vagrant'
    timeout 6000
    environment 'TOOLCHAIN' => 'arm_v5t_le-gcc.tar.gz'
    code <<-EOC
        [ -e "$TOOLCHAIN" ] && rm -f $TOOLCHAIN
        if curl -O http://192.168.2.152/public/toolchain/$TOOLCHAIN; then
            sudo tar -zxf $TOOLCHAIN -C /opt
            rm -f $TOOLCHAIN
            echo -n "export PATH=$" | cat > ti-toolchain.sh
            echo "PATH:/opt/mv_pro_5.0/montavista/pro/devkit/arm/v5t_le/bin/" | cat >> ti-toolchain.sh
            sudo mv ti-toolchain.sh /etc/profile.d/
        fi
    EOC
    creates "/opt/mv_pro_5.0"
end