#!/bin/bash
set -e

BLD_PATH=`pwd`/${0%`basename $0`}
[ ${0##/*} ] || BLD_PATH=${0%`basename $0`}
BLD_PATH=${BLD_PATH%*/}

# must set src path
if [ "${bamboo_srcpath}" ]; then
    SRCPATH=${bamboo_srcpath}
else
    SRCPATH="${BLD_PATH}/.."
fi

#black=$(tput setaf 0)
#red=$(tput setaf 1)
#green=$(tput setaf 2)
#blue=$(tput setaf 4)

#DONE="${green}DONE${black}"
#FAIL="${red}FAIL${black}"
DONE="DONE"
FAIL="FAIL"

MODULE_LIST="svn_update ubl bootloader stellaris encmp kernel ipnc_sdk rootfs rootfs_df firmware build_debug nfs rootfs_nogpl clean"
ARGS=${MODULE_LIST}
[ $# -gt 0 ] && ARGS=$@

check_auth() {
    echo -n "Checking authorize..."
    sudo test /bin/sh
    if [ $? -ne 0 ]; then
        echo ${FAIL}
        return 1
    fi
    echo ${DONE}
    return 0
}

svn_update() {
    if ! svn info 2>&- 1>&-; then
        return 0
    fi

    [ -f "${SRCPATH}/.account" ] && . ${SRCPATH}/.account
    if [ ! -d ${SRCPATH} ]; then
        echo "Counldn't find \"${SRCPATH}\" directory."

        if [ "${bamboo_svnpath}" ]; then
            echo "Starting checkout from \"${bamboo_svnpath}\"..."
            svn co ${bamboo_svnpath} ${SRCPATH} --non-interactive --trust-server-cert ${ACCOUNT}
            if [ $? -ne 0 ]; then
                echo ${FAIL}
                return 1
            fi
        else
            echo "Fail to checkout from \"${bamboo_svnpath}\""
            return 1
        fi
    fi

    echo -n "Updating source files from \"$SRCPATH\"..."

    /usr/bin/svn info $SRCPATH --non-interactive --trust-server-cert ${ACCOUNT}
    if [ $? -ne 0 ]; then
        echo ${FAIL}
        return 1
    fi
    /usr/bin/svn up $SRCPATH --force --non-interactive --trust-server-cert ${ACCOUNT}
    if [ $? -ne 0 ]; then
        echo ${FAIL}
        return 1
    fi
    echo ${DONE}

    return 0
}

firmware() {
    local maxsize=31457280
    local basic_path
    local buildnum
    echo -n "Making Firmware..."

    if [ -z "${bamboo_buildNumber}" ]; then
        basic_path="${SRCPATH}/install"
    else
        basic_path="${SRCPATH}/install.${bamboo_buildNumber}"
    fi

    if [ ! -d "${basic_path}" ]; then
        echo "the images is not installed"
        exit 1
    fi

    [ "${bamboo_buildNumber}" ] && cp ${SRCPATH}/install/makeFirmware.sh ${basic_path}/

    cd ${basic_path}
    ./makeFirmware.sh auto
    if [ $? -ne 0 ]; then
        echo ${FAIL}
        cd ${BLD_PATH}
        return 1
    fi
    echo ${DONE}

    if [ -z "${bamboo_buildNumber}" ]; then
        [ -f "${basic_path}/.buildnum" ] || echo "0" > ${basic_path}/.buildnum
        read prebuildnum < ${basic_path}/.buildnum
        buildnum="I${prebuildnum}"
    else
        buildnum="${bamboo_buildNumber}"
    fi

    cd ${BLD_PATH}
    local firmware_file=`ls -t1 ${basic_path}/_firmware/*${buildnum}*.enc | head -n2`
    local firmwarebin_file=`ls -t1 ${basic_path}/_firmware/*${buildnum} | head -n2`
    local firmware_size_list=`ls -l ${firmware_file} | awk '{print $5}'`
    for firmware_size in ${firmware_size_list}
    do
        if [ ${firmware_size} -gt ${maxsize} ]; then
            echo ${FAIL}
        echo " - Firmware(${firmware_size}) size must less than ${maxsize}"	
        return 1
        fi
    done

    [ -d ./firmware ] && rm -rf ./firmware
    mkdir -p firmware
    cp ${firmware_file} ./firmware/

    [ -d ./firmware ] && rm -rf ./firmwarebin
    mkdir -p firmwarebin
    cp ${firmwarebin_file} ./firmwarebin/

    rm -rf ./imagefiles
    cp -dpR ${basic_path}/_firmware/*${buildnum}.images ./imagefiles 2>&-

    return 0
}

build_debug() {
    echo -n "Debug Build for Rootfs..."
    cd ${SRCPATH}/rootfs

    DEBUG=y make clean all install

    if [ $? -ne 0 ]; then
        echo ${FAIL}
        echo " - Failed to build ${image}"
        cd ${bld_path}
        return 1
    fi

    if [ ! -z "${nfs_dir}" ]; then
        rm -rf "${nfs_dir}.tmp"
        cp -dpR ${SRCPATH}/build/rootfs ${nfs_dir}.tmp

        DEBUG=y DEBUGTOOLS_INSDIR=${nfs_dir}.tmp make install_debug_tools
        sudo tar --wildcards -zxf ${SRCPATH}/rootfs/init/rootfs.tgz -C ${nfs_dir}.tmp/ dev/*

        if [ $? -ne 0 ]; then
            echo ${FAIL}
            echo " - Failed to build ${image}"
            cd ${bld_path}
            return 1
        fi
    fi

    echo ${DONE}
}

nfs () {
    local install_dir="${SRCPATH}/$(ls -t1 ${SRCPATH} | grep install | head -n 1)"
    local kernel_install_dir="${install_dir}/kernel/$(ls -t1 ${install_dir}/kernel|head -n 1)"
    local rootfs_install_dir="${nfs_dir}.tmp"

#    cd ${SRCPATH}/rootfs
#    DEBUG=y DEBUGTOOLS_INSDIR=${rootfs_install_dir} make install_debug_tools
#    sudo tar --wildcards -zxf ${SRCPATH}/rootfs/init/rootfs.tgz -C ${rootfs_install_dir}/ dev/*

#    if [ $? -ne 0 ]; then
#        echo ${FAIL}
#        echo " - Failed to build ${image}"
#        cd ${bld_path}
#        return 1
#    fi

    if [ ! -z "${tftp_dir}" ]; then
        [ -f "${kernel_install_dir}/ipx_kernel.bin" ] && cp -f ${kernel_install_dir}/ipx_kernel.bin ${tftp_dir}
    fi

    if [ ! -z "${nfs_dir}" ]; then
        if [ ! -d "${rootfs_install_dir}" ]; then
            echo ${FAIL}
            echo " - Can't find rootfs path(${rootfs_install_dir})"
            return 1
        fi
        [ -d "${nfs_dir}" ] || mkdir -p ${nfs_dir}
        sudo rm -rf ${nfs_dir}/*
        sudo cp -dpR ${rootfs_install_dir}/* ${nfs_dir}/
    fi
}

build_image() {
    local image=$1

    local log_path="${BLD_PATH}/log"
    local logout="${log_path}/${image}.out.log"
    local logerr="${log_path}/${image}.err.log"
    local buildpath="${SRCPATH}/${image}"

    if [ $# -ne 1 ]; then
        echo "It has to a argument"
        return 1
    fi

    #create build log directory
    [ -d ${log_path} ] || mkdir ${log_path} -p

    #no-gpl
    if [ "${image}" = "rootfs_nogpl" ]; then
        buildpath="${SRCPATH}/rootfs"
    else
        if [ ! -d "${buildpath}" ]; then
            echo "It isn't directory or couldn't be found.(${buildpath})"
            return 0
        fi
    fi

    echo -n "Building ${image}..."
    local bld_run="install.sh"
    [ ${image%rootfs*} ] || bld_run="automake.sh"
    [ "${image}" = "rootfs_nogpl" ] && bld_option="no-gpl"

    if [ ! -x "${buildpath}/${bld_run}" ]; then
        echo ${FAIL}
        echo " - Count't execute shell (${buildpath}/${bld_run})"
        return 1
    fi

    cd ${buildpath}
    ./${bld_run} no-update $bld_option

    if [ $? -ne 0 ]; then
        echo ${FAIL}
        echo " - Failed to build ${image}"
        cd ${bld_path}
        return 1
    fi
    echo ${DONE}

    return 0
}

clean() {
    local dir="${SRCPATH}"
    if svn info 2>&- 1>&-; then
        svn revert -R ${dir} && svn cleanup ${dir} && svn st ${dir} | grep ? | sed 's/^?       //g' | xargs rm -rf && svn up ${dir}
    else
        cd ${dir};git clean -xfd .
    fi
	echo $dir
    return 0
}

for module in ${ARGS}
do
    if [[ "${MODULE_LIST}" = ${module}\ * ]] || 
       [[ "${MODULE_LIST}" = *\ ${module} ]] ||
       [[ "${MODULE_LIST}" = *\ ${module}\ * ]]
    then
        if [ "${module}" = "svn_update" ] ||
           [ "${module}" = "firmware" ] ||
           [ "${module}" = "clean" ] ||
           [ "${module}" = "build_debug" ] ||
           [ "${module}" = "nfs" ]
        then
            if ! ${module}; then
                echo "Fail to run ${module}!!"
                exit -1
            fi
        else
            if ! build_image ${module}; then
                echo "Fail build ${module}!!"
                exit -1
            fi
        fi
    else
        echo "Can't find any modules - ${module}"
        continue
    fi
done

exit 0
