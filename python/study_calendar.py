"""
调用pyecharts生成专注时间日历图
"""
import datetime
import pandas as pd

from pyecharts import options as opts
from pyecharts.charts import Calendar
from pyecharts.render import make_snapshot
from snapshot_selenium import snapshot


# 数据分析
p = pd.read_csv("c:/users/sanw/desktop/hours.csv")
hours = p.hours.values.tolist()
sum_hours = sum(hours)
day = len(hours)
avg_hours = sum_hours / day
if day < 30:
    avg_hours30 = avg_hours
else:
    avg_hours30 = sum(hours[-30:]) / 30
sub = "共" + str(day) + "天计" + str(sum_hours) + "小时 平均" + str(
    round(avg_hours, 2)) + "小时 近30天平均" + str(round(avg_hours30, 2)) + "小时"


# 日历图
def calendar_base() -> Calendar:
    begin = datetime.date(2020, 2, 12)
    data = [[str(begin + datetime.timedelta(days=i)), hours[i]]
            for i in range(day)]

    c = (Calendar(init_opts=opts.InitOpts(width='1200px', height='300px', bg_color='white'))
        .add("", data, calendar_opts=opts.CalendarOpts(range_="2020"))
        .set_global_opts(
            title_opts=opts.TitleOpts(
                title="小吴-2020年专注情况",
                subtitle=sub,
                item_gap=2,
                pos_left='center',
                padding=3),
            visualmap_opts=opts.VisualMapOpts(
                max_=15,
                min_=0,
                orient="horizontal",
                is_piecewise=True,
                pos_top="220px",
                pos_left="100px",
                ),
            ))
    return c


# 生成图片
make_snapshot(snapshot, calendar_base().render(), "c://users/sanw/desktop/calendar.png")
print("生成完毕")