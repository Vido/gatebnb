from django.contrib import admin
from django.utils.html import format_html, mark_safe
from django.contrib.sites.shortcuts import get_current_site
from django.contrib.sites.models import Site

from import_export.admin import ExportMixin
from import_export import resources

from .models import GuestAccess, GateAccessLog

class GateAccessLogResource(resources.ModelResource):
    class Meta:
        model = GateAccessLog
        fields = ("id", "triggered_at", "action", "status", "guest_access__name")

class GateAccessLogInline(admin.TabularInline):
    model = GateAccessLog
    extra = 0
    readonly_fields = ("triggered_at", "action", "status_colored")
    can_delete = False

    def status_colored(self, obj):
        color = {
            "allowed": "green",
            "denied": "red",
        }.get(obj.status, "gray")
        return format_html('<span style="color: {};">{}</span>', color, obj.status.capitalize())
    status_colored.short_description = "Status"


@admin.register(GuestAccess)
class GuestAccessAdmin(admin.ModelAdmin):
    list_display = ("name", "start_time", "end_time", "block", 'access_url_link')
    list_filter = ("start_time", "end_time", 'block')
    search_fields = ("name", "phone", "email", "token")
    readonly_fields = ("created_at", "modified_on", "token")
    inlines = [GateAccessLogInline]

    def is_active_display(self, obj):
        return obj.is_active()

    is_active_display.short_description = "Active?"
    is_active_display.boolean = True

    def access_url_link(self, obj):
        site = Site.objects.get_current()
        url = mark_safe(obj.get_url())
        label = mark_safe(site.domain + obj.get_url())
        return format_html('<a href="{}" target="_blank">{}</a>', url, label)

    access_url_link.short_description = "Access Link"

@admin.register(GateAccessLog)
class GateAccessLogAdmin(ExportMixin, admin.ModelAdmin):
    resource_class = GateAccessLogResource
    list_display = ("triggered_at", "guest_name", "action", "status_colored")
    list_filter = ("status", "action", "triggered_at")
    search_fields = ("guest_access__name", "guest_access__phone", "action", "status")
    ordering = ("-triggered_at",)

    def guest_name(self, obj):
        return obj.guest_access.name if obj.guest_access else "Anonymous"

    def status_colored(self, obj):
        color = {
            "allowed": "green",
            "denied": "red",
        }.get(obj.status, "gray")
        return format_html('<span style="color: {};">{}</span>', color, obj.status.capitalize())
    status_colored.short_description = "Status"
