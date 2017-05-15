#!/usr/bin/python

mesice = {
    'Leden':1, 'Unor':2, 'Brezen':3, 'Duben':4, 
    'Kveten':5, 'Cerven':6,'Cervenec':7, 'Srpen':8,
    'Zari':9, 'Rijen':10, 'Listopad':11, 'Prosinec':12}

date_day = ''
date_month = ''
time_vychod = ''
time_zapad = ''

next_vychod = False
next_zapad  = False
day_complete = False

buff = []

with open('rawtimetable.txt') as f:
    for line in f:
        l = line.strip()

        if day_complete:
            day_complete = False
            vychod_hour = str(int(time_vychod[0:2]))
            vychod_min = str(int(time_vychod[3:5]))
            zapad_hour = str(int(time_zapad[0:2]))
            zapad_min = str(int(time_zapad[3:5]))
            record = ','.join([ str(date_month), str(date_day), vychod_hour, vychod_min, zapad_hour, zapad_min, ])
            print '{ ' + record + ' },'

        elif next_vychod:
            time_vychod = l
            next_vychod = False
        elif next_zapad:
            time_zapad = l
            next_zapad = False
            day_complete = True
        elif l in mesice:
            date_month = mesice[l]
            # date_month = l
            pass
        elif l == '2017':
            pass
        elif l.endswith('.'):
            pass
        
        if l == 'Vychod:':
            next_vychod = True
        elif l == 'Zapad:':
            next_zapad = True
        else:
            try:
                date_day = int(l)
            except:
                pass

