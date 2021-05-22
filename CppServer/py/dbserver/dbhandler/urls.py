from django.urls import path
from .views import *

urlpatterns = [
    path('api/create_user', RegisterUser.as_view(), name='create_user'),
    path('api/users', UserView.as_view(), name='users'),
    path('', home_page, name='home'),
]