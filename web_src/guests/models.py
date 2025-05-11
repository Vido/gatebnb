import uuid

from django.db import models
from django.utils import timezone
from django.urls import reverse
from django.core.exceptions import ValidationError


def _validate_phone(value):
    from phonenumbers import parse, is_valid_number
    if not is_valid_number(parse(value, "BR")):
         raise ValidationError("Invalid phone number")

def _normalize_phone(value):
    from phonenumbers import parse, format_number, PhoneNumberFormat
    parsed = parse(value, "BR")
    return format_number(parsed, PhoneNumberFormat.E164)

class GuestAccess(models.Model):

    name = models.CharField(max_length=100)
    phone = models.CharField(
        max_length=20,
        blank=True,
        validators=[_validate_phone]
    )
    email = models.EmailField(blank=True, null=True)
    token = models.CharField(max_length=64, unique=True, editable=False)
    
    block = models.BooleanField(default=False)
    start_time = models.DateTimeField()
    end_time = models.DateTimeField()

    created_at = models.DateTimeField(auto_now_add=True, editable=False)
    modified_on = models.DateTimeField(auto_now=True, editable=False)

    def __str__(self):
        return f"{self.name} ({self.start_time.date()} - {self.end_time.date()})"

    def save(self, *args, **kwargs):
        if not self.token:
            self.token = uuid.uuid4().hex
        if self.phone:
            self.phone = _normalize_phone(self.phone)
        super().save(*args, **kwargs)

    def is_active(self):
        now = timezone.now()
        return self._is_active_logic(now) and not self.block 

    def _is_active_logic(self, now):
        return self.start_time <= now <= self.end_time

    def get_url(self):
        return reverse('autorize_gate', kwargs={'token': self.token})

ACTION_CHOICES = [
    ("open", "Open Gate"),
    ("close", "Close Gate"),
]

STATUS_CHOICES = [
    ("allowed", "Allowed"),
    ("denied", "Denied"),
]


class GateAccessLog(models.Model):

    guest_access = models.ForeignKey(
        GuestAccess,
        on_delete=models.PROTECT,
        related_name='access_logs',
        editable=False,
        blank=True,
        null=True
    )

    triggered_at = models.DateTimeField(auto_now_add=True, editable=False)
    action = models.CharField(max_length=10, choices=ACTION_CHOICES, editable=False)
    status = models.CharField(max_length=10, choices=STATUS_CHOICES, editable=False)

    def __str__(self):
        return f"{self.action.capitalize()} - {self.status.capitalize()} - {self.guest_access.name} at {self.triggered_at}"
