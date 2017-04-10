# -*- coding: utf-8 -*-
# Generated by Django 1.9 on 2017-01-04 17:37
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('cUSB', '0001_initial'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='journalmodel',
            name='eventtime',
        ),
        migrations.AddField(
            model_name='journalmodel',
            name='dtime',
            field=models.DateTimeField(null=True, verbose_name='Время'),
        ),
        migrations.AddField(
            model_name='journalmodel',
            name='event',
            field=models.CharField(max_length=1024, null=True, verbose_name='Описание'),
        ),
        migrations.AlterField(
            model_name='journalmodel',
            name='computername',
            field=models.CharField(max_length=512, null=True, verbose_name='Имя компьютера'),
        ),
        migrations.AlterField(
            model_name='journalmodel',
            name='device_id',
            field=models.CharField(max_length=512, null=True, verbose_name='Идентификатор'),
        ),
        migrations.AlterField(
            model_name='journalmodel',
            name='username',
            field=models.CharField(max_length=512, null=True, verbose_name='Имя пользователя'),
        ),
    ]