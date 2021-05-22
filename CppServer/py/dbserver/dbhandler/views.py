from django.shortcuts import render
from django.http import HttpResponse
from rest_framework.views import *
from .serializers import * 
import datetime
from rest_framework.response import Response
from rest_framework import status
from rest_framework.permissions import AllowAny
from rest_framework import generics

def home_page(request):
    now = datetime.datetime.now()
    return render(request, template_name='index.html', 
                context={'now': now})


class RegisterUser(APIView):
    serializer_class = UserSerializer
    permission_classes = (AllowAny,)

    def post(self, request, format=None):
        data = {}
        ser_json = {}
        username   = request.data['username']
        first_name = request.data['first_name']
        last_name  = request.data['last_name']
        email      = request.data['email']
        password   = request.data['password']

        user = User.objects.create(username=username,first_name=first_name,
                last_name=last_name, email=email, password=password)
        if user:
            data['success'] = True
            data['result']  = True
            return Response(data=data, status=status.HTTP_200_OK)
        else:
            data['success'] = False
            data['result']  = False
            return Response(data=data, status=status.HTTP_400_BAD_REQUEST)

class UserView(generics.ListAPIView):
    serializer_class = UserSerializer
    queryset = User.objects.all()
    permission_classes = (AllowAny, )

    