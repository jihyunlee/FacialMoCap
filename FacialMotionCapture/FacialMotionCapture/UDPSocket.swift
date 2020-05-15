//
//  UDPSocket.swift
//  FacialMotionCapture
//
//  Created by Jihyun Lee on 5/5/20.
//  Copyright © 2020 Jihyun Lee. All rights reserved.
//

import Foundation

@_silgen_name("udpsocket_open") func c_udpsocket_open() -> Int32
@_silgen_name("udpsocket_close") func c_udpsocket_close(_ socketId:Int32) -> Int32
@_silgen_name("udpsocket_enable_broadcast") func c_udpsocket_enable_broadcast(_ socketId:Int32) -> Void
@_silgen_name("udpsocket_send") func c_udpsocket_send(_ socketId:Int32,buff:UnsafePointer<UInt8>,ip:UnsafePointer<Int8>,port:Int32) -> Int32
@_silgen_name("udpsocket_broadcast") func c_udpsocket_broadcast(_ socketId:Int32,buff:UnsafePointer<UInt8>,port:Int32) -> Int32

@objc open class UDPBroadcaster: NSObject {
    
    var ip: String
    var port: Int
    var socketId: Int32?
    var ipAddresses: [String] = []
    
    init(port: Int) {
        self.ip = ""
        self.port = port
        
        let socketId:Int32 = c_udpsocket_open()
        if socketId > 0 {
            self.socketId = socketId
        }
    }
    
    init(ip :String, port :Int) {
        self.ip = ip
        self.port = port

        let socketId:Int32 = c_udpsocket_open()
        if socketId > 0 {
            self.socketId = socketId
        }
    }
    
    public func setIpAddresses(array: [String]) {
        self.ipAddresses = array
    }

    public func enableBroadcast() {
        if let socketId:Int32 = self.socketId {
            c_udpsocket_enable_broadcast(socketId)
        }
    }
    
    public func send(_ string: String)->(Bool,String) {
        if string.count <= 9216 {
            
            if let socketId:Int32 = self.socketId {
                let ret:Int32 = c_udpsocket_send(socketId, buff: string, ip:self.ip, port: Int32(self.port))
                if ret == Int32(strlen(string)) {
                    return (true,"send success")
                } else {
                    return (false,"send error")
                }
            } else{
                return (false,"socket not open")
            }
        }
        
        print("packet is too large. Must be smaller than 9200 bytes")
        return (false, "packet is too large")
    }
    
    public func sendToList(_ string: String) {
        for addr in self.ipAddresses {
            if string.count <= 9216 {
                if let socketId:Int32 = self.socketId {
                    _ = c_udpsocket_send(socketId, buff: string, ip:addr, port: Int32(self.port))
                }
            }
        }
    }

    public func broadcast(_ string: String)->(Bool,String) {
        if string.count <= 9216 {
            
            if let socketId:Int32 = self.socketId {
                let ret:Int32 = c_udpsocket_broadcast(socketId, buff: string, port: Int32(self.port))
                if ret == Int32(strlen(string)) {
                    return (true,"send success")
                } else {
                    return (false,"send error")
                }
            } else{
                return (false,"socket not open")
            }
        }
        
        print("packet is too large. Must be smaller than 9200 bytes")
        return (false, "packet is too large")
    }

    deinit {
        if let socketId:Int32=self.socketId{
            _ = c_udpsocket_close(socketId)
            self.socketId = nil
        }
    }
}

