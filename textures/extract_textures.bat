cd textures

set cnt=0
for %%A in (*) do set /a cnt+=1

echo %cnt% > ..\textures.txt
dir /b /a-d >> ..\textures.txt


..\converter.exe
