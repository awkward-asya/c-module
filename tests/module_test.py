import seam_carving
import seam_carving_module


filename1 = "little-pied-cormorant-g74d730a07_1920.jpg"
dw1 = 500
dh1 = 300

filename2 = "beautiful-cats-cat-s.jpg"
dw2 = 30
dh2 = 30

filename3 = "kitty.jpg"
dw3 = 200
dh3 = 100

filename4 = "suspicious-kitty.jpg"
dw4 = 200
dh4 = 100

filename5 = "/Users/asenkasazonova/Documents/универ/4_семестр/си/module/test/kitty.jpg"
dw5 = 100
dh5 = 50

filename6 = "beautiful-cats-cat-s.jpg"
dw6 = 50
dh6 = 50

test_number = 3
test_filename = filename3
test_dw = dw3
test_dh = dh3
print("\ntest ", test_number, "\nfilename:", test_filename)
seam_carving_module.resize_image(test_filename, test_dw, test_dh)
