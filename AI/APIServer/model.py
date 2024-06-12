import torch 
import torch.nn as nn 
from torchvision.models import resnet50, ResNet50_Weights 

class ResNetClassification(nn.Module):
  def __init__(self, dropout= 0.1):
      super().__init__()
      resnet= resnet50(weights= ResNet50_Weights.IMAGENET1K_V2)
      resnet.requires_grad_(True)
      self.encoder= nn.Sequential(*list(resnet.children()))[:-1]
      
      self.dropout= nn.Dropout(dropout)
      self.fc= nn.Linear(2048, 4)

  def forward(self, x):
    x= self.encoder(x).view(-1, 2048)
    x= self.dropout(x)
    return self.fc(x)