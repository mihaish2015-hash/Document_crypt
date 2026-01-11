echo -e -n "What is the file path ?\nAnswer: "
read -r path
if echo $path | grep -q '"'; then
    path=${path//\"/}
fi
if [ ! -f "$path" ]; then
    echo "File does not exist !"
    exit 1
fi
echo -e -n "What action do you want to do ?\n1. Encrypt\n2. Decrypt\nAnswer: "
read act
echo -e -n "What key do you want to use ?\nAnswer: "
read key
if [[ $path == *.txt ]]; then
    ./cript  "$path" "$act" "$key"
else
    echo "Only .txt files can be encrypted/decrypted !"
    exit 1
fi
echo "Operation successful !"
echo -n "Press ENTER to exit..."
read nothing
