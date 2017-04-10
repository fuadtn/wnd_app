from django.db import models


class ListModel(models.Model):
    device_id = models.CharField(max_length=512, verbose_name='Идентификатор')

    class Meta:
        verbose_name = 'Список зарегистрированных USB-устройств'
        verbose_name_plural = 'Список зарегистрированных USB-устройств'
        db_table = 'usb_list'


class JournalModel(models.Model):
    device_id = models.CharField(max_length=512, verbose_name='Идентификатор', null=True)
    username = models.CharField(max_length=512, verbose_name='Имя пользователя', null=True)
    computername = models.CharField(max_length=512, verbose_name='Имя компьютера', null=True)
    dtime = models.DateTimeField(verbose_name='Время', null=True)
    event = models.CharField(max_length=1024, verbose_name='Описание', null=True)

    class Meta:
        verbose_name = 'Журнал событий'
        verbose_name_plural = 'Журнал событий'
        db_table = 'usb_journal'