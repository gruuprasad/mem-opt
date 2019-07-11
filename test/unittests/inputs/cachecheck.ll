; ModuleID = 'cachecheck.c'
source_filename = "cachecheck.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.pkt_tcp = type <{ %struct.eth_hdr, %struct.ip_hdr, %struct.tcp_hdr }>
%struct.eth_hdr = type { %struct.eth_addr, %struct.eth_addr, %struct.beui16 }
%struct.eth_addr = type { [6 x i8] }
%struct.beui16 = type { i16 }
%struct.ip_hdr = type { i8, i8, %struct.beui16, %struct.beui16, %struct.beui16, i8, i8, i16, %struct.beui32, %struct.beui32 }
%struct.beui32 = type { i32 }
%struct.tcp_hdr = type { %struct.beui16, %struct.beui16, %struct.beui32, %struct.beui32, i16, %struct.beui16, i16, %struct.beui16 }
%struct.flow_key = type { %struct.beui32, %struct.beui32, %struct.beui16, %struct.beui16 }

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @cache_fn1(i32*) #0 {
  %2 = alloca i32*, align 8
  %3 = alloca %struct.pkt_tcp*, align 8
  %4 = alloca %struct.flow_key, align 1
  store i32* %0, i32** %2, align 8
  %5 = call noalias i8* @malloc(i64 54) #3
  %6 = bitcast i8* %5 to %struct.pkt_tcp*
  store %struct.pkt_tcp* %6, %struct.pkt_tcp** %3, align 8
  %7 = getelementptr inbounds %struct.flow_key, %struct.flow_key* %4, i32 0, i32 0
  %8 = load %struct.pkt_tcp*, %struct.pkt_tcp** %3, align 8
  %9 = getelementptr inbounds %struct.pkt_tcp, %struct.pkt_tcp* %8, i32 0, i32 1
  %10 = getelementptr inbounds %struct.ip_hdr, %struct.ip_hdr* %9, i32 0, i32 9
  %11 = bitcast %struct.beui32* %7 to i8*
  %12 = bitcast %struct.beui32* %10 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %11, i8* align 1 %12, i64 4, i1 false)
  %13 = getelementptr inbounds %struct.flow_key, %struct.flow_key* %4, i32 0, i32 1
  %14 = load %struct.pkt_tcp*, %struct.pkt_tcp** %3, align 8
  %15 = getelementptr inbounds %struct.pkt_tcp, %struct.pkt_tcp* %14, i32 0, i32 1
  %16 = getelementptr inbounds %struct.ip_hdr, %struct.ip_hdr* %15, i32 0, i32 8
  %17 = bitcast %struct.beui32* %13 to i8*
  %18 = bitcast %struct.beui32* %16 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %17, i8* align 1 %18, i64 4, i1 false)
  %19 = getelementptr inbounds %struct.flow_key, %struct.flow_key* %4, i32 0, i32 2
  %20 = load %struct.pkt_tcp*, %struct.pkt_tcp** %3, align 8
  %21 = getelementptr inbounds %struct.pkt_tcp, %struct.pkt_tcp* %20, i32 0, i32 2
  %22 = getelementptr inbounds %struct.tcp_hdr, %struct.tcp_hdr* %21, i32 0, i32 1
  %23 = bitcast %struct.beui16* %19 to i8*
  %24 = bitcast %struct.beui16* %22 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %23, i8* align 1 %24, i64 2, i1 false)
  %25 = getelementptr inbounds %struct.flow_key, %struct.flow_key* %4, i32 0, i32 3
  %26 = load %struct.pkt_tcp*, %struct.pkt_tcp** %3, align 8
  %27 = getelementptr inbounds %struct.pkt_tcp, %struct.pkt_tcp* %26, i32 0, i32 2
  %28 = getelementptr inbounds %struct.tcp_hdr, %struct.tcp_hdr* %27, i32 0, i32 0
  %29 = bitcast %struct.beui16* %25 to i8*
  %30 = bitcast %struct.beui16* %28 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %29, i8* align 1 %30, i64 2, i1 false)
  %31 = load %struct.pkt_tcp*, %struct.pkt_tcp** %3, align 8
  %32 = bitcast %struct.pkt_tcp* %31 to i8*
  call void @free(i8* %32) #3
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i1) #2

; Function Attrs: nounwind
declare void @free(i8*) #1

attributes #0 = { noinline nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { argmemonly nounwind }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
