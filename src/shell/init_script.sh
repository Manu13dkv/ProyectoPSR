#!/bin/bash
#
#
#me="dit@10.1.10.2"
master="dit@10.1.1.2"
#worker1="dit@10.1.2.2"
#worker2="dit@10.1.3.2"

#config_file="${HOME}/develop/ns3/proyecto_psr/assets/config_files/proxy.conf"
#config_path="/etc/httpd/conf.d/"

#scp -p ${me}:${config_file} ${master}:${config_path}

#ssh $master "sudo systemctl restart httpd"

for i in $( seq 1 1000 )
do
    curl http://10.1.1.2/lorem_ipsum.html
done
 