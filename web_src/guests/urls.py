from django.urls import path
from django.views.generic import TemplateView

from . import views

urlpatterns = [
    path('', TemplateView.as_view(template_name="index.html"), name="index"),
    path('gate/<str:token>/', TemplateView.as_view(template_name="gate.html"), name='autorize_gate'),
    path('gate/<str:token>/<str:action>', views.autorize_gate, name='open_gate'),
]
