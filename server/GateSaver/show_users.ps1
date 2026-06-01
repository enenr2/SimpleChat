
Add-Type -Path 'C:\Program Files (x86)\MySQL\MySQL Installer for Windows\MySql.Data.dll'
$conn = New-Object MySql.Data.MySqlClient.MySqlConnection('server=127.0.0.1;port=3306;uid=root;pwd=123456;database=enenr')
$conn.Open()
$cmd = $conn.CreateCommand()
$cmd.CommandText = "SELECT uid, name, email FROM user"
$r = $cmd.ExecuteReader()
while($r.Read()){
    Write-Output "uid=$($r['uid'])  name=$($r['name'])  email=$($r['email'])"
}
$r.Close()
$conn.Close()
