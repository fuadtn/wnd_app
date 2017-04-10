from django.shortcuts import render
from django.http import HttpResponse
from django.contrib.auth import authenticate
from django.views.decorators.csrf import csrf_exempt
from .models import ListModel, JournalModel
from rest_framework import status
import datetime

@csrf_exempt
def autorization(request):
    if request.method == 'POST':
        username = request.POST.get('username')
        password = request.POST.get('password')

        if(username is not None) or (password is not None):
            login = authenticate(username=username, password=password)

            if login is not None:
                return HttpResponse(status=status.HTTP_200_OK)
            else:
                return HttpResponse(status=status.HTTP_401_UNAUTHORIZED)
    return HttpResponse(status=status.HTTP_204_NO_CONTENT)


@csrf_exempt
def send_list(request):
    if request.method == 'POST':
        devices = request.POST.get('devices')
        if devices is not None:
            s_devices = devices.split('  ')
            for sd in s_devices:
                list_object = ListModel()
                list_object.device_id = sd
                list_object.save()
            return HttpResponse(status=status.HTTP_200_OK)
    return HttpResponse(status=status.HTTP_204_NO_CONTENT)


def get_list(request):
    objects = ListModel.objects.all()
    if objects is not None:
        s_result = ''
        for o in objects:
            s_result += o.device_id + '\n'
        return HttpResponse(s_result, content_type='text/plain', status=status.HTTP_200_OK)
    return HttpResponse(status=status.HTTP_204_NO_CONTENT)


@csrf_exempt
def send_event(request):
    if request.method == 'POST':
        event = JournalModel()
        event.dtime = datetime.datetime.now()
        event.username = request.POST.get('username')
        event.computername = request.POST.get('computername')
        event.device_id = request.POST.get('device_id')
        event.event = request.POST.get('event')
        event.save()
    else:
        return HttpResponse(status=status.HTTP_204_NO_CONTENT)
