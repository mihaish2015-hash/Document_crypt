Add-Type -AssemblyName System.IO.Compression.FileSystem
function zip ($Path, $File) {
    try {
        $Z = [System.IO.Compression.ZipFile]::Open($Path, "Update")
        $Int = $File
        if ($E = $Z.GetEntry($Int)) { $E.Delete() }
        [void][System.IO.Compression.ZipFileExtensions]::CreateEntryFromFile($Z, (Resolve-Path $File).Path, $Int)
        $Z.Dispose()
    }
    catch {
        Write-Host "An error occurred !"
        Write-Host "Operation failed !" -ForegroundColor Red
        if (Test-Path "word") {
            Remove-Item "word" -Recurse -Force
        }
        if (Test-Path "ppt") {
            Remove-Item "ppt" -Recurse -Force
        }
        if (Test-Path "xl") {
            Remove-Item "xl" -Recurse -Force
        }
        if (Test-Path "tmp.txt") {
            Remove-Item "tmp.txt" -Force
        }
        Read-Host "Press ENTER to exit..."
        exit
    }
}
function unzip ($Path, $File) {
    try {
        $Z = [System.IO.Compression.ZipFile]::OpenRead($Path)
        $Dest = "$PWD\$File"
        New-Item -ItemType Directory -Force -Path (Split-Path $Dest) | Out-Null
        [System.IO.Compression.ZipFileExtensions]::ExtractToFile($Z.GetEntry($File), $Dest, $true)
        $Z.Dispose()
    }
    catch {
        Write-Host "An error occurred !"
        Write-Host "Operation failed !" -ForegroundColor Red
        if (Test-Path "word") {
            Remove-Item "word" -Recurse -Force
        }
        if (Test-Path "ppt") {
            Remove-Item "ppt" -Recurse -Force
        }
        if (Test-Path "xl") {
            Remove-Item "xl" -Recurse -Force
        }
        if (Test-Path "tmp.txt") {
            Remove-Item "tmp.txt" -Force
        }
        Read-Host "Press ENTER to exit..."
        exit
    }
}

Write-Host "What is the file path ?`nAnswer: " -NoNewline
$path = Read-Host
$path = $path -replace '"', ''
if (-not (Test-Path -Path $path -PathType Leaf)) {
    Write-Host "File does not exist !"
    Write-Host "Operation failed !" -ForegroundColor Red
    Read-Host "Press Enter to exit..."
    exit
}
Write-Host "What action do you want to do ?`n1. Encrypt`n2. Decrypt`nAnswer: " -NoNewline
$act = Read-Host
if($act -ne "1" -and $act -ne "2") {
    Write-Host "You must type 1 or 2 !"
    Write-Host "Operation failed !" -ForegroundColor Red
    Read-Host "Press Enter to exit..."
    exit
}
Write-Host "What key do you want to use ?`nAnswer: " -NoNewline
$key = Read-Host
$suc=1
if($path -like "*.txt") {
    #& path\to\your\cript.exe $path $act $key
    if($LASTEXITCODE -eq 1) {
        $suc=0
    }
}
elseif($path -like "*.docx") {
    $cur_dir = Get-Location
    unzip "$path" word/document.xml
    New-Item tmp.txt | Out-Null
    #path\to\your\extract.exe "$cur_dir\word\document.xml" "$cur_dir\tmp.txt"
    if($LASTEXITCODE -eq 1) {
        $suc=0
    }
    #path\to\your\cript.exe "$cur_dir\tmp.txt" "$act" "$key"
    if($LASTEXITCODE -eq 1) {
        $suc=0
    }
    #path\to\your\inject.exe "$cur_dir\tmp.txt" "$cur_dir\word\document.xml"
    zip "$path" word/document.xml
    Remove-Item word -Recurse
    Remove-Item tmp.txt
}
elseif($path -like "*.pptx") {
    $cur_dir = Get-Location
    $tempZip = [System.IO.Compression.ZipFile]::OpenRead($path)
    $slideEntries = $tempZip.Entries | Where-Object { $_.FullName -like "ppt/slides/slide*.xml" }
    $tempZip.Dispose()
    foreach ($entry in $slideEntries) {
        unzip $path $entry.FullName
    }
    $Slides = Get-ChildItem ".\ppt\slides\slide*.xml"
    $nr = $Slides.Count
    for($i = 1; $i -le $nr; $i++) {
        New-Item tmp.txt | Out-Null
        #path\to\your\extract.exe "$cur_dir\ppt\slides\slide$i.xml" "$cur_dir\tmp.txt"
        if($LASTEXITCODE -eq 1) {
            $suc=0
        }
        #path\to\your\cript.exe "$cur_dir\tmp.txt" "$act" "$key"
        if($LASTEXITCODE -eq 1) {
            $suc=0
        }
        #path\to\your\inject.exe "$cur_dir\tmp.txt" "$cur_dir\ppt\slides\slide$i.xml"
        zip "$path" ppt/slides/slide$i.xml
        Remove-Item tmp.txt
    }
    Remove-Item ppt -Recurse
}
elseif($path -like "*.xlsx") {
    $cur_dir = Get-Location
    unzip "$path" xl/sharedStrings.xml
    New-Item tmp.txt | Out-Null
    #path\to\your\extract.exe "$cur_dir\xl\sharedStrings.xml" "$cur_dir\tmp.txt"
    if($LASTEXITCODE -eq 1) {
        $suc=0
    }
    #path\to\your\cript.exe "$cur_dir\tmp.txt" "$act" "$key"
    if($LASTEXITCODE -eq 1) {
        $suc=0
    }
    #path\to\your\inject.exe "$cur_dir\tmp.txt" "$cur_dir\xl\sharedStrings.xml"
    zip "$path" xl/sharedStrings.xml
    Remove-Item xl -Recurse
    Remove-Item tmp.txt
}
else {
    Write-Host "Only .txt, .docx, .pptx, .xlsx files can be encrypted/decrypted !"
    $suc=0
}

if($suc -eq 1) {
    Write-Host "Operation successful !" -ForegroundColor Green
}
else {
    Write-Host "Operation failed !" -ForegroundColor Red
}
Read-Host "Press ENTER to exit..."
exit
