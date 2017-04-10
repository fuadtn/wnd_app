from django.contrib import admin
from .models import ListModel, JournalModel


class ListAdmin(admin.ModelAdmin):
    list_display = ('device_id',)
    list_filter = ('device_id',)


class JournalAdmin(admin.ModelAdmin):
    list_display = ('dtime', 'computername', 'username', 'device_id', 'event')
    list_filter = ('dtime', 'computername',)

    def has_add_permission(self, request):
        return False


admin.site.register(ListModel, ListAdmin)
admin.site.register(JournalModel, JournalAdmin)
