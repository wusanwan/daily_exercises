from wxpy import *
from random import *
import requests
import time,datetime
from threading import Timer
import requests
from apscheduler.schedulers.blocking import BlockingScheduler


#登录
bot = Bot(console_qr = -1,cache_path=True)
myself = bot.self
bot.file_helper.send('上线了！')
bot.enable_puid()

#图灵bot
tuling = Tuling(api_key='xxxxxxxxxxxxxxxxxxx')
url_api = 'http://www.tuling123.com/openai/api'


#对象
dorm_group=ensure_one(bot.groups().search("水晶女孩走了"))
lecture=ensure_one(bot.mps().search('wusanwan'))


##监听
#自动回复/用来骗表情包
@bot.register(bot.friends(),enabled = False)
def autoreply(msg):
    return 'Bot:已收到消息{}'.format(msg.text)

#宿舍群监听
@bot.register(dorm_group, TEXT,except_self=False,enabled=True)
def reply_group(msg):
    if msg.is_at:
        tuling.do_reply(msg)
    elif msg.text=="【今天吃什么】":
        return("QT:"+what_to_eat())
    elif msg.text=="【天气】":
        return(get_weather())
    elif msg.text=="【生活】":
        return(get_life_suggestion())
    elif msg.text=="【老黄历】":
        return(get_calendar())
    elif msg.text=="【课表】":
        return(get_courses())
    else:
        return

      return
    
#公众号更新提醒
@bot.register(lecture,enabled=True)
def warning_in_dorm(msg):
    dorm_group.send('Robot:wusanwan公众号更新了！')
    

#今天吃什么
def what_to_eat():
    food_list=["香锅","焗饭","周先生的饭","炒面","盖浇饭","寿司","石锅拌饭","肉夹馍","酸辣粉","炸鸡饭","盐酥鸡饭","鸡公煲",'炸鸡']
    random_number=randint(0,len(food_list)-1)
    return food_list[random_number]

##定时框架
#定时提醒   
def life_struggles():
    time=datetime.datetime.now()
    new_time = str(time)
    hour = new_time[11:13]
    if int(hour) <7 or int(hour)>23:
       return
     bot.file_helper.send("Bot:该学习了！")


#待办事项准时提醒
def yiban_warning():
     dorm_group.send('QT:易班签到时间到！')

#每日提醒
def countdown():
    now=datetime.datetime.now()
    d1=datetime.datetime(2019,5,11)
    d2=datetime.datetime(2019,6,30)
    d3=datetime.datetime(2019,9,8)
    d4=datetime.datetime(2019,9,21)
    d5=datetime.datetime(2019,12,21)
    diff1=(d1-now).days
    diff2=(d2-now).days
    diff3=(d3-now).days
    diff4=(d4-now).days
    diff5=(d5-now).days
    text="QT:【倒计时】：JAVA考试"+str(diff1)+"天\n第一阶段结束"+str(diff2)+"天\nPAT"+str(diff3)+"天\n高数竞赛"+str(diff4)+"天\n考研"+str(diff5)+"天"
    return text

def get_weather():
    payload = {'key':'xxxxxxxxxxxxxxxx','location':'xxxxxxxxx'}
    pollution= requests.get("https://free-api.heweather.net/s6/air/now", params=payload,timeout=1)
    
    payload1 = {'key':'xxxxxxxxxxxxxxxx','location':'xxxxxxxx','days':'1'}
    result=requests.get("https://api.seniverse.com/v3/weather/daily.json",params=payload1,timeout=1)

    pollution=pollution.json()
    result_pollution=pollution['HeWeather6']
    result_pollution=result_pollution[0]
    air_now_city=result_pollution['air_now_city']

     
    result=result.json()
    results=result['results']
    results=results[0]
    daily=results['daily']
    daily=daily[0]

    location=results['location']
        
    weather_text='QT:【天气情况】当前城市为'+location['name']+'，'+daily['text_day']+'转'+daily['text_night']+'，最高温度:'+daily['high']+'摄氏度，最低温度：'+daily['low']+'摄氏度，风力等级为'+daily['wind_scale']+'空气污染指数为'+air_now_city['aqi']+'，空气污染质量为'+air_now_city['qlty']
    return weather_text

def get_life_suggestion():
    payload = {'key':'xxxxxxxxxxxxxxxxxxx','location':'xxxxxxxxxxxx'}
    result= requests.get("xxxxxxxxxxxxxxxxxxxxxxxxxxxx", params=payload,timeout=1)
    result=result.json()
    result=result['HeWeather6']
    result=result[0]
    suggestion=result['lifestyle']
    location=result['basic']
    suggestion_text='QT:【生活指数】当前城市为'+location['location']+'\n舒适：'+suggestion[0]['txt']+',\n 穿衣：'+suggestion[1]['txt']+'\n 流感：'+suggestion[2]['txt']+'\n 运动：'+suggestion[3]['txt']+'\n 紫外线：'+suggestion[5]['txt']+'\n 空气情况：'+suggestion[7]['txt']
    return suggestion_text

def get_calendar():
    payload = {'key':'xxxxxxxxxxxxxxxxx','date':time.strftime('%y-%m-%d')}
    calendar= requests.get("http://v.juhe.cn/laohuangli/d", params=payload,timeout=1)
    result=calendar.json()
    result=result['result']
    text="QT:【老黄历】今天是"+result['yinli']+"\n宜："+result['yi']+"\n忌："+result['ji']
    return text

def get_courses():
    now=datetime.datetime.now()
    start=datetime.datetime(2019,2,25)
    diff=(now-start).days
    week=int(diff/7)+1
    day=diff%7+1
    data=[[[10,11,"图形学 1,2 N8421"],[9,13,"软工 3,4 N8323"]],
          [[9,15, "Web  1,2 N8619"],[9,15,"Web  3,4 一层南"]],
          [[9,0,"图形学 3,4 N8423"]],
          [[9,12,"软工 1,2 N8421"],[13,0,"软工 1,2 N8224"],[9,15,"Web  3,4 N8604"]],
          [[9,11,"图形学 1,2 N8626"],[9,12,"软工 3,4 二层南"]]]
    text=""
    if(day>5):
        return
    else :
        result=data[day-1]
        for temp in result:
          start_week=temp[0]
          end_week=temp[1]
          if(end_week==0 and start_week==week or week<= end_week and start_week<=week):
              text+=temp[2]+"\n"
    if(text==""):return"QT:【课表】今日无课"
    else:return "QT:【课表】"+text


def daily_report():
    dorm_group.send(get_weather())
    dorm_group.send(get_life_suggestion())
    dorm_group.send(get_calendar())
    dorm_group.send(get_courses())

def self_driving():
    bot.file_helper.send(countdown())

#进程
sched = BlockingScheduler()
sched.add_job(self_driving,'interval',seconds=3600)
sched.add_job(daily_report,'cron',month='1-12',day='1-31',hour=10,minute=15)
sched.add_job(yiban_warning,'cron',day_of_week='sun',hour=18,minute=0)
sched.add_job(life_struggles,'interval',seconds=7200)
sched.start()
bot.join()

