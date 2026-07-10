curl 192.168.1.100:8080/generate/source/basilisk-core.xml -o basilisk-core-generated.zip
tar xf basilisk-core-generated.zip -C basilisk-core/generated
del basilisk-core-generated.zip