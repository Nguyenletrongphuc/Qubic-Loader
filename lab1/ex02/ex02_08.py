def chiahetcho5(sonhiphan):
    sothapphan=int(sonhiphan,2)
    if sothapphan%5==0:
        return True
    else:
        return False
chuoisonhiphan = input("Nhap chuoi nhi phan(phan tach boi dau phay): ")
sonhiphan_list=chuoisonhiphan.split(',')
sochiahetcho5 = [so for so in sonhiphan_list if chiahetcho5(so)]
if len(sochiahetcho5)>0:
    ketqua = ','.join(sochiahetcho5)
    print("cac so nhi phan chia het cho 5 la: ",ketqua)
else:
    print("khong co so nhi phan nao chia het cho 5")

