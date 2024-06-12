import cv2 
import torch 
import requests 
from model import ResNetClassification 
from constant import URL_CAM

def get_image():
    response= requests.get(f'{URL_CAM}/capture')
    if response.status_code== 200: 
        with open('data/image.jpg', 'wb') as file:
            file.write(response.content)
        print('Image saved successfully')
    else:
        raise ValueError('An error occurred: {e}')

def preprocess_img(input_size= (224, 224)): 
    img= cv2.imread('data/image.jpg')
    img= cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    img= cv2.resize(img, input_size, interpolation= cv2.INTER_LINEAR)
    img= img/255. 

    return img.transpose(2, 0, 1).astype("float32")

def load_model(path_weight): 
    model= ResNetClassification()
    model.load_state_dict(
        torch.load(
            path_weight, 
            map_location="cpu"
        )['model_state_dict']
    )

    return model