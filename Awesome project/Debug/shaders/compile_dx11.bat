shadercDebug.exe -i ./ -f ./%1/fs_%1.sc --type f --platform windows -p ps_4_0 -O 3 -o ./dx11/fs_%1.bin
shadercDebug.exe -i ./ -f ./%1/vs_%1.sc --type v --platform windows -p vs_4_0 -O 3 -o ./dx11/vs_%1.bin
