from django.test import TestCase
from django.utils import timezone
from datetime import timedelta

from .models import GuestAccess, GateAccessLog


class GuestAccessModelTests(TestCase):

    def test_guest_creation_generates_token_and_normalizes_phone(self):
        guest = GuestAccess.objects.create(
            name="João da Silva",
            phone="(11) 91234-5678",
            start_time=timezone.now(),
            end_time=timezone.now() + timedelta(days=2),
        )
        self.assertTrue(guest.token)
        self.assertTrue(guest.phone.startswith("+55"))

    def test_guest_is_active(self):
        now = timezone.now()
        guest = GuestAccess.objects.create(
            name="Active Guest",
            phone="+5511912345678",
            start_time=now - timedelta(hours=1),
            end_time=now + timedelta(hours=1),
        )
        self.assertTrue(guest.is_active())

    def test_guest_is_inactive_before_start(self):
        now = timezone.now()
        guest = GuestAccess.objects.create(
            name="Future Guest",
            phone="+5511912345678",
            start_time=now + timedelta(days=1),
            end_time=now + timedelta(days=2),
        )
        self.assertFalse(guest.is_active())

    def test_guest_is_inactive_after_end(self):
        now = timezone.now()
        guest = GuestAccess.objects.create(
            name="Past Guest",
            phone="+5511912345678",
            start_time=now - timedelta(days=2),
            end_time=now - timedelta(days=1),
        )
        self.assertFalse(guest.is_active())
