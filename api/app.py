from fastapi import FastAPI, Request
from bson import ObjectId
from fastapi.middleware.cors import CORSMiddleware 
from fastapi import FastAPI, HTTPException
from datetime import datetime
import motor.motor_asyncio
import pydantic
import requests
import pytz

app = FastAPI()

origins=[
    "http://localhost:8000",
   ## "https://ecse3038-lab3-tester.netlify.app"
   "https://ecse-sunset-api.onrender.com/"
]

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


pydantic.json.ENCODERS_BY_TYPE[ObjectId]=str

client = motor.motor_asyncio.AsyncIOMotorClient("mongodb+srv://week4:oKbzC9wuJa6nIJKI@cluster0.ldjatx3.mongodb.net/?retryWrites=true&w=majority")

db = client.sunset

@app.get("/api/state")
async def getapistate():
  state=await db["api"].find_one({"jah": "kno"})

  if state is not None:
    
     return state
  if state is None:
    return {"fan":False, "light": False}
  return state


@app.put("/api/state",status_code=204)
async def putstate(request: Request):
    ps= await request.json()
    sunset = requests.get('https://ecse-sunset-api.onrender.com/api/sunset').json()

    ##profile_obj["last_updated"]=datetime.now()
    sunset_time = datetime.strptime(sunset['sunset'], '%Y-%m-%dT%H:%M:%S.%f').time()
    ct_object1 = datetime.now(pytz.timezone('Jamaica')).time()
    dt_object1 = datetime.strptime(str(sunset_time), "%d/%m/%Y %H:%M:%S")
    dt_object2 = datetime.strptime(str(ct_object1), "%d/%m/%Y %H:%M:%S")

    ps["light"] = (dt_object2>dt_object1)
    ps["fan"] = (float(ps["temperature"]) >= 28.0) 

    check = await db["api"].find_one({"jah": "kno"})
    if check is not None:
      await db["api"].update_one(({"jah": "kno"}), {"$set": ps})
      check = await db["api"].find_one({"jah": "kno"})
    else:
     await db["api"].insert_one({**ps, "jah": "kno"})
     check = await db["api"].find_one({"jah": "kno"})
