import cv2
import numpy as np

#initialize the camera
cap = cv2.VideoCapture(0)

#Face detection
face_cascade = cv2.CascadeClassifier("haarcascade_frontalface_alt.xml")
face_data = []
skip = 0

print('Rotate your complete face slowly !!!!!')
file_name = input('Enter name of the person: ')

while True:
    ret,frame = cap.read()
    if ret == False:
        continue
    
    gray = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)

    faces = face_cascade.detectMultiScale(gray,1.3,5)
    faces = sorted(faces,key = lambda f: f[2]*f[3])

    for face in faces[-1:]:
        x,y,w,h = face
        cv2.rectangle(frame,(x,y),(x+w,y+h),(0,255,255),2)

        #extract : Region of interest
        offset = 10
        face_section = frame[y-offset:y+h+offset,x-offset:x+w+offset]
        face_section = cv2.resize(face_section,(100,100))

        if(skip%10==0):
            face_data.append(face_section)
            print('.',end = ' ')
        cv2.imshow("Face Section",face_section)
    skip += 1
    cv2.imshow("frame",frame)
    

    key_pressed = cv2.waitKey(1) & 0xFF
    if len(face_data)==30:
        break

#convert our face data into numpy array
face_data = np.asarray(face_data)
face_data = face_data.reshape((face_data.shape[0],-1))
print(face_data.shape)

np.save(file_name+'.npy',face_data)
print('face data succesfully saved for ',file_name)

cap.release()
cv2.destroyAllWindows()
