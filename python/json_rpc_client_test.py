#!/usr/bin/env python
# -*- coding: utf-8 -*-
# vim: set ts=2 expandtab:
'''
Module: json_rpc_client_test.py
Desc: make json rpc calls on C++ video overlay pipeline
Author: on_three
Email: on.three.email@gmail.com
DATE: Sunday, June 1st 2014

python test to make json rpc calls on video overlay pipeline.

[
    {	//This is how you define a method with named parameters.
        "method": "StaticMessage",
        "params": {
            "msg": "Hello.",
			"name" : "1",
			"x" : 0,
			"y" : 0,
			"w" : 100,
			"h" : 100
        },
        "returns": "Success"
    }
]
  
'''

import pyjsonrpc
import argparse

def main():
  parser = argparse.ArgumentParser(description='JSON RPC calls on gstreamer pipeline test.')
  parser.add_argument('message', help='Test to display on video stream', type=str)
  parser.add_argument('-x', help='x pos of message on screen in pixels', type=int, default=100)
  parser.add_argument('-y', help='y pos of message on screen in pixels', type=int, default=100)
  #parser.add_argument('pid', help='Pid of closed caption ES to extract from stream.', type=int)
  #parser.add_argument('-m', '--message_name', help='Friendly name to remove/change this message later.', type=str)
  args = parser.parse_args()

  http_client = pyjsonrpc.HttpClient(
    url = 'http://localhost:8080',
    #username = 'Username',
    #password = 'Password',
  )

  msg = args.message
  x = args.x
  y = args.y
  #friendly_name = args.message_name or 'test message'
  #response = http_client.call('ShowMessage', msg, x, y, friendly_name)
  response = http_client.StaticMessage(msg=msg, name="1", x=x, y=y, w=100, h=100)

  print('Response from server: {r}'.format(r=response))
    

if __name__ == "__main__":
  main()