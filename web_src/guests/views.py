from django.shortcuts import render

from django.utils import timezone
from django.http import JsonResponse, HttpResponseBadRequest, HttpResponseNotFound
from .models import GuestAccess, GateAccessLog


def autorize_gate(request, token, action):
    guest = GuestAccess.objects.filter(token=token).first()

    status = "denied"
    valid_action = action in ["open", "close"]

    if guest and valid_action and guest.is_active():
        status = "allowed"

    log_action = action if valid_action else "invalid"

    GateAccessLog.objects.create(
        guest_access=guest,
        action=log_action,
        status=status,
    )

    if not valid_action:
        response = JsonResponse({"status": "denied", "message": "Invalid action."}, status=400)
    elif guest is None:
        response = JsonResponse({"status": "denied", "message": "Invalid token."}, status=404)
    elif status == "allowed":
        response =  JsonResponse({"status": "success", "message": f"Gate {action}ed."})
    else:
        response = JsonResponse({"status": "denied", "message": "Access time expired or not yet active."}, status=403)
    return response
