sogiolam = float(input("nhap so gio lam moi tuan: "))
luonggio = float(input("nhap thu lao moi gio: "))
giotieuchuan = 44
giovuotchuan = max(0 , sogiolam - giotieuchuan)
thuclinh = giotieuchuan* luonggio+giovuotchuan*luonggio*1.5
print(f"so tien thuc linh cua nhan vien: {thuclinh}")