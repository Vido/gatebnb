from django.urls import path
from django.views.generic import TemplateView

from . import views

urlpatterns = [
    path('', TemplateView.as_view(template_name="index.html"), name="index"),
    path('gate/<str:token>/', views.GateView.as_view(), name='autorize_gate'),
    path('gate/<str:token>/<str:action>', views.autorize_gate, name='open_gate'),
]
