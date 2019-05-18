from PIL import Image
import win32ui

dlg=win32ui.CreateFileDialog(1)
dlg.SetOFNInitialDir("file:///C:/Users/sanw/Desktop")
dlg.DoModal()
filename=dlg.GetPathName()
image=Image.open(filename)
width,height=image.size
is_at_a_ratio=True;
if is_at_a_ratio:
    ratio=width/height;
    new_height=200
    new_width=int(new_height*ratio)
    #new_width=200
    #new_height=int(new_width/ratio)
    image=image.resize((new_width,new_height))
else:
    image=image.resize((100,200))
image.save(filename)