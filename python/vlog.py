'''

三十行代码实现整个文件夹的视频拼接，并加速到60s，最后加上字幕输出。
'''

from moviepy.editor import *
    import datetime
    import os

    # 从本地载入视频并合成
    start = datetime.datetime.now()
    path = r"e:\vlog\today"
    clips = []
    file_list = os.listdir(path)
    for files in file_list:  # 命名的时候按顺序命名哈
        complete_path = os.path.join(path, files)
        print(complete_path)
        clip = VideoFileClip(complete_path, target_resolution=(270, 480)) # 调整大小
        clip = clip.volumex(0)
        clips.append(clip)
    clip = concatenate_videoclips(clips)

    # 加速
    duration = 60
    clip = clip.fx(vfx.speedx, final_duration=duration)

    # 字幕
    today="0308"
    # today = datetime.datetime.today().strftime("%m%d") # 如果当天的话用这个
    txt_clip = TextClip(today,
                        fontsize=90,
                        color='white',
                        stroke_color='#00373D',
                        stroke_width=2)
    txt_clip = txt_clip.set_pos('center').set_duration(5)
    video = CompositeVideoClip([clip, txt_clip])

    video.write_videofile("e:/vlog/" + today + ".mp4", audio=False,fps=24)
    end = datetime.datetime.now()

    print("total used:", (end - start).seconds / 60, "mins")    
