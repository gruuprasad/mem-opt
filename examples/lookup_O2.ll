; ModuleID = 'lookup.c'
source_filename = "lookup.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.packet = type { i32, i64 }

@hashes = common dso_local local_unnamed_addr global [8192 x i32] zeroinitializer, align 16
@packet_buffer = common dso_local local_unnamed_addr global [48 x %struct.packet] zeroinitializer, align 16
@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.1 = private unnamed_addr constant [10 x i8] c"tas_batch\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [9 x i8] c"lookup.c\00", section "llvm.metadata"
@expected = common dso_local local_unnamed_addr global [48 x i32] zeroinitializer, align 16
@llvm.global.annotations = appending global [1 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (void (%struct.packet*)* @hash_lookup to i8*), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.2, i32 0, i32 0), i32 31 }], section "llvm.metadata"

; Function Attrs: norecurse nounwind sspstrong uwtable
define dso_local void @fill_hashTable() local_unnamed_addr #0 {
  br label %1

; <label>:1:                                      ; preds = %1, %0
  %2 = phi i64 [ 0, %0 ], [ %30, %1 ]
  %3 = phi <4 x i32> [ <i32 0, i32 1, i32 2, i32 3>, %0 ], [ %31, %1 ]
  %4 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %2
  %5 = add <4 x i32> %3, <i32 4, i32 4, i32 4, i32 4>
  %6 = bitcast i32* %4 to <4 x i32>*
  store <4 x i32> %3, <4 x i32>* %6, align 16, !tbaa !4
  %7 = getelementptr inbounds i32, i32* %4, i64 4
  %8 = bitcast i32* %7 to <4 x i32>*
  store <4 x i32> %5, <4 x i32>* %8, align 16, !tbaa !4
  %9 = or i64 %2, 8
  %10 = add <4 x i32> %3, <i32 8, i32 8, i32 8, i32 8>
  %11 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %9
  %12 = add <4 x i32> %3, <i32 12, i32 12, i32 12, i32 12>
  %13 = bitcast i32* %11 to <4 x i32>*
  store <4 x i32> %10, <4 x i32>* %13, align 16, !tbaa !4
  %14 = getelementptr inbounds i32, i32* %11, i64 4
  %15 = bitcast i32* %14 to <4 x i32>*
  store <4 x i32> %12, <4 x i32>* %15, align 16, !tbaa !4
  %16 = or i64 %2, 16
  %17 = add <4 x i32> %3, <i32 16, i32 16, i32 16, i32 16>
  %18 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %16
  %19 = add <4 x i32> %3, <i32 20, i32 20, i32 20, i32 20>
  %20 = bitcast i32* %18 to <4 x i32>*
  store <4 x i32> %17, <4 x i32>* %20, align 16, !tbaa !4
  %21 = getelementptr inbounds i32, i32* %18, i64 4
  %22 = bitcast i32* %21 to <4 x i32>*
  store <4 x i32> %19, <4 x i32>* %22, align 16, !tbaa !4
  %23 = or i64 %2, 24
  %24 = add <4 x i32> %3, <i32 24, i32 24, i32 24, i32 24>
  %25 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %23
  %26 = add <4 x i32> %3, <i32 28, i32 28, i32 28, i32 28>
  %27 = bitcast i32* %25 to <4 x i32>*
  store <4 x i32> %24, <4 x i32>* %27, align 16, !tbaa !4
  %28 = getelementptr inbounds i32, i32* %25, i64 4
  %29 = bitcast i32* %28 to <4 x i32>*
  store <4 x i32> %26, <4 x i32>* %29, align 16, !tbaa !4
  %30 = add nuw nsw i64 %2, 32
  %31 = add <4 x i32> %3, <i32 32, i32 32, i32 32, i32 32>
  %32 = icmp eq i64 %30, 8192
  br i1 %32, label %33, label %1, !llvm.loop !8

; <label>:33:                                     ; preds = %1
  ret void
}

; Function Attrs: norecurse nounwind sspstrong uwtable
define dso_local void @fill_packet_buffer(%struct.packet** nocapture readnone, i32) local_unnamed_addr #0 {
  %3 = icmp sgt i32 %1, 0
  br i1 %3, label %4, label %17

; <label>:4:                                      ; preds = %2
  %5 = zext i32 %1 to i64
  %6 = and i64 %5, 1
  %7 = icmp eq i32 %1, 1
  br i1 %7, label %10, label %8

; <label>:8:                                      ; preds = %4
  %9 = sub nsw i64 %5, %6
  br label %18

; <label>:10:                                     ; preds = %18, %4
  %11 = phi i64 [ 0, %4 ], [ %28, %18 ]
  %12 = icmp eq i64 %6, 0
  br i1 %12, label %17, label %13

; <label>:13:                                     ; preds = %10
  %14 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %11, i32 0
  %15 = trunc i64 %11 to i32
  store i32 %15, i32* %14, align 16, !tbaa !10
  %16 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %11, i32 1
  store i64 32, i64* %16, align 8, !tbaa !13
  br label %17

; <label>:17:                                     ; preds = %13, %10, %2
  ret void

; <label>:18:                                     ; preds = %18, %8
  %19 = phi i64 [ 0, %8 ], [ %28, %18 ]
  %20 = phi i64 [ %9, %8 ], [ %29, %18 ]
  %21 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %19, i32 0
  %22 = trunc i64 %19 to i32
  store i32 %22, i32* %21, align 16, !tbaa !10
  %23 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %19, i32 1
  store i64 32, i64* %23, align 8, !tbaa !13
  %24 = or i64 %19, 1
  %25 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %24, i32 0
  %26 = trunc i64 %24 to i32
  store i32 %26, i32* %25, align 16, !tbaa !10
  %27 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %24, i32 1
  store i64 32, i64* %27, align 8, !tbaa !13
  %28 = add nuw nsw i64 %19, 2
  %29 = add i64 %20, -2
  %30 = icmp eq i64 %29, 0
  br i1 %30, label %10, label %18
}

; Function Attrs: nounwind sspstrong uwtable
define dso_local void @hash_lookup(%struct.packet* nocapture readonly) #1 {
  %2 = getelementptr inbounds %struct.packet, %struct.packet* %0, i64 0, i32 0
  %3 = load i32, i32* %2, align 8, !tbaa !10
  %4 = sext i32 %3 to i64
  %5 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %4
  %6 = load i32, i32* %5, align 4, !tbaa !4
  %7 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32 %6)
  ret void
}

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #2

; Function Attrs: nounwind sspstrong uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
  %1 = tail call noalias i8* @malloc(i64 384) #3
  %2 = bitcast i8* %1 to %struct.packet**
  br label %18

; <label>:3:                                      ; preds = %25, %3
  %4 = phi i64 [ %16, %3 ], [ 0, %25 ]
  %5 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %4, i32 0
  %6 = trunc i64 %4 to i32
  store i32 %6, i32* %5, align 16, !tbaa !10
  %7 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %4, i32 1
  store i64 32, i64* %7, align 8, !tbaa !13
  %8 = add nuw nsw i64 %4, 1
  %9 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %8, i32 0
  %10 = trunc i64 %8 to i32
  store i32 %10, i32* %9, align 16, !tbaa !10
  %11 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %8, i32 1
  store i64 32, i64* %11, align 8, !tbaa !13
  %12 = add nuw nsw i64 %4, 2
  %13 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %12, i32 0
  %14 = trunc i64 %12 to i32
  store i32 %14, i32* %13, align 16, !tbaa !10
  %15 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %12, i32 1
  store i64 32, i64* %15, align 8, !tbaa !13
  %16 = add nuw nsw i64 %4, 3
  %17 = icmp eq i64 %16, 48
  br i1 %17, label %57, label %3

; <label>:18:                                     ; preds = %18, %0
  %19 = phi i64 [ 0, %0 ], [ %23, %18 ]
  %20 = tail call noalias i8* @malloc(i64 16) #3
  %21 = getelementptr inbounds %struct.packet*, %struct.packet** %2, i64 %19
  %22 = bitcast %struct.packet** %21 to i8**
  store i8* %20, i8** %22, align 8, !tbaa !14
  %23 = add nuw nsw i64 %19, 1
  %24 = icmp eq i64 %23, 48
  br i1 %24, label %25, label %18

; <label>:25:                                     ; preds = %18, %25
  %26 = phi i64 [ %54, %25 ], [ 0, %18 ]
  %27 = phi <4 x i32> [ %55, %25 ], [ <i32 0, i32 1, i32 2, i32 3>, %18 ]
  %28 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %26
  %29 = add <4 x i32> %27, <i32 4, i32 4, i32 4, i32 4>
  %30 = bitcast i32* %28 to <4 x i32>*
  store <4 x i32> %27, <4 x i32>* %30, align 16, !tbaa !4
  %31 = getelementptr inbounds i32, i32* %28, i64 4
  %32 = bitcast i32* %31 to <4 x i32>*
  store <4 x i32> %29, <4 x i32>* %32, align 16, !tbaa !4
  %33 = or i64 %26, 8
  %34 = add <4 x i32> %27, <i32 8, i32 8, i32 8, i32 8>
  %35 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %33
  %36 = add <4 x i32> %27, <i32 12, i32 12, i32 12, i32 12>
  %37 = bitcast i32* %35 to <4 x i32>*
  store <4 x i32> %34, <4 x i32>* %37, align 16, !tbaa !4
  %38 = getelementptr inbounds i32, i32* %35, i64 4
  %39 = bitcast i32* %38 to <4 x i32>*
  store <4 x i32> %36, <4 x i32>* %39, align 16, !tbaa !4
  %40 = or i64 %26, 16
  %41 = add <4 x i32> %27, <i32 16, i32 16, i32 16, i32 16>
  %42 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %40
  %43 = add <4 x i32> %27, <i32 20, i32 20, i32 20, i32 20>
  %44 = bitcast i32* %42 to <4 x i32>*
  store <4 x i32> %41, <4 x i32>* %44, align 16, !tbaa !4
  %45 = getelementptr inbounds i32, i32* %42, i64 4
  %46 = bitcast i32* %45 to <4 x i32>*
  store <4 x i32> %43, <4 x i32>* %46, align 16, !tbaa !4
  %47 = or i64 %26, 24
  %48 = add <4 x i32> %27, <i32 24, i32 24, i32 24, i32 24>
  %49 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %47
  %50 = add <4 x i32> %27, <i32 28, i32 28, i32 28, i32 28>
  %51 = bitcast i32* %49 to <4 x i32>*
  store <4 x i32> %48, <4 x i32>* %51, align 16, !tbaa !4
  %52 = getelementptr inbounds i32, i32* %49, i64 4
  %53 = bitcast i32* %52 to <4 x i32>*
  store <4 x i32> %50, <4 x i32>* %53, align 16, !tbaa !4
  %54 = add nuw nsw i64 %26, 32
  %55 = add <4 x i32> %27, <i32 32, i32 32, i32 32, i32 32>
  %56 = icmp eq i64 %54, 8192
  br i1 %56, label %3, label %25, !llvm.loop !16

; <label>:57:                                     ; preds = %3, %57
  %58 = phi i64 [ %67, %57 ], [ 0, %3 ]
  %59 = getelementptr inbounds %struct.packet*, %struct.packet** %2, i64 %58
  %60 = load %struct.packet*, %struct.packet** %59, align 8, !tbaa !14
  %61 = getelementptr inbounds %struct.packet, %struct.packet* %60, i64 0, i32 0
  %62 = load i32, i32* %61, align 8, !tbaa !10
  %63 = sext i32 %62 to i64
  %64 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %63
  %65 = load i32, i32* %64, align 4, !tbaa !4
  %66 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32 %65) #3
  %67 = add nuw nsw i64 %58, 1
  %68 = icmp eq i64 %67, 48
  br i1 %68, label %70, label %57

; <label>:69:                                     ; preds = %70
  tail call void @free(i8* nonnull %1) #3
  ret i32 0

; <label>:70:                                     ; preds = %57, %70
  %71 = phi i64 [ %75, %70 ], [ 0, %57 ]
  %72 = getelementptr inbounds %struct.packet*, %struct.packet** %2, i64 %71
  %73 = bitcast %struct.packet** %72 to i8**
  %74 = load i8*, i8** %73, align 8, !tbaa !14
  tail call void @free(i8* %74) #3
  %75 = add nuw nsw i64 %71, 1
  %76 = icmp eq i64 %75, 48
  br i1 %76, label %69, label %70
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) local_unnamed_addr #2

; Function Attrs: nounwind
declare void @free(i8* nocapture) local_unnamed_addr #2

attributes #0 = { norecurse nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 7.0.1 (tags/RELEASE_701/final)"}
!4 = !{!5, !5, i64 0}
!5 = !{!"int", !6, i64 0}
!6 = !{!"omnipotent char", !7, i64 0}
!7 = !{!"Simple C/C++ TBAA"}
!8 = distinct !{!8, !9}
!9 = !{!"llvm.loop.isvectorized", i32 1}
!10 = !{!11, !5, i64 0}
!11 = !{!"packet", !5, i64 0, !12, i64 8}
!12 = !{!"long", !6, i64 0}
!13 = !{!11, !12, i64 8}
!14 = !{!15, !15, i64 0}
!15 = !{!"any pointer", !6, i64 0}
!16 = distinct !{!16, !9}
