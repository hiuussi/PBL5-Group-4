import torch 
import os 
from fastapi import FastAPI, HTTPException 
from fastapi.middleware.cors import CORSMiddleware

from preprocess import preprocess_img, get_image, load_model
from constant import PATH_WEIGHT 

os.environ['KMP_DUPLICATE_LIB_OK']='TRUE'

device= torch.device('cuda' if torch.cuda.is_available() else 'cpu')
dtype= torch.float16 if torch.cuda.is_available() else torch.float32 

model= load_model(PATH_WEIGHT)
model.to(device, dtype= dtype)

app = FastAPI(
    title= "Classification Trash"
)
app.add_middleware(
    CORSMiddleware,
    allow_origins= ['*'],
    allow_credentials= True, 
    allow_methods= ['*'],
    allow_headers= ['*'],
)

@app.post('/classify', tags= ['Classify'])
def classify(): 
    try:
        get_image()
        img= torch.from_numpy(preprocess_img((224, 224))).unsqueeze(0)

        with torch.no_grad():
            output= model(img.to(device))
            output= output.argmax(-1).view(-1, )

        return output.item()
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))
