set SERVER=192.168.1.100
set USERNAME=reno
set REMOTE_DIR=/opt/basilisk-nginx/xml

scp xml\* "%USERNAME%@%SERVER%:%REMOTE_DIR%/"

ssh %USERNAME%@%SERVER% "sudo /usr/bin/systemctl reload basilisk-nginx"

curl http://192.168.1.100:8080/generate/source?addons=core,gfx,mod -o generated.zip

rmdir /s /q temp_extract 2>nul
mkdir temp_extract

tar xf generated.zip -C temp_extract

xcopy /K /D /H /Y temp_extract\core\* basilisk-core\generated\

xcopy /K /D /H /Y temp_extract\gfx\* basilisk-gfx\generated\

xcopy /K /D /H /Y temp_extract\mod\* basilisk-mod\generated\

rmdir /s /q temp_extract
del generated.zip
