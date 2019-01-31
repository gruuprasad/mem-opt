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
  br label %2

; <label>:1:                                      ; preds = %2
  ret void

; <label>:2:                                      ; preds = %2, %0
  %3 = phi i64 [ 0, %0 ], [ %6, %2 ]
  %4 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %3
  %5 = trunc i64 %3 to i32
  store i32 %5, i32* %4, align 4, !tbaa !4
  %6 = add nuw nsw i64 %3, 1
  %7 = icmp eq i64 %6, 8192
  br i1 %7, label %1, label %2
}

; Function Attrs: norecurse nounwind sspstrong uwtable
define dso_local void @fill_packet_buffer(%struct.packet** nocapture readnone, i32) local_unnamed_addr #0 {
  %3 = icmp sgt i32 %1, 0
  br i1 %3, label %4, label %6

; <label>:4:                                      ; preds = %2
  %5 = zext i32 %1 to i64
  br label %7

; <label>:6:                                      ; preds = %7, %2
  ret void

; <label>:7:                                      ; preds = %7, %4
  %8 = phi i64 [ 0, %4 ], [ %12, %7 ]
  %9 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %8, i32 0
  %10 = trunc i64 %8 to i32
  store i32 %10, i32* %9, align 16, !tbaa !8
  %11 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %8, i32 1
  store i64 32, i64* %11, align 8, !tbaa !11
  %12 = add nuw nsw i64 %8, 1
  %13 = icmp eq i64 %12, %5
  br i1 %13, label %6, label %7
}

; Function Attrs: nounwind sspstrong uwtable
define dso_local void @hash_lookup(%struct.packet* nocapture readonly) #1 {
  %2 = getelementptr inbounds %struct.packet, %struct.packet* %0, i64 0, i32 0
  %3 = load i32, i32* %2, align 8, !tbaa !8
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
  br label %4

; <label>:3:                                      ; preds = %4
  tail call void @fill_hashTable()
  tail call void @fill_packet_buffer(%struct.packet** undef, i32 48)
  br label %11

; <label>:4:                                      ; preds = %4, %0
  %5 = phi i64 [ 0, %0 ], [ %9, %4 ]
  %6 = tail call noalias i8* @malloc(i64 16) #3
  %7 = getelementptr inbounds %struct.packet*, %struct.packet** %2, i64 %5
  %8 = bitcast %struct.packet** %7 to i8**
  store i8* %6, i8** %8, align 8, !tbaa !12
  %9 = add nuw nsw i64 %5, 1
  %10 = icmp eq i64 %9, 48
  br i1 %10, label %3, label %4

; <label>:11:                                     ; preds = %11, %3
  %12 = phi i64 [ 0, %3 ], [ %15, %11 ]
  %13 = getelementptr inbounds %struct.packet*, %struct.packet** %2, i64 %12
  %14 = load %struct.packet*, %struct.packet** %13, align 8, !tbaa !12
  tail call void @hash_lookup(%struct.packet* %14)
  %15 = add nuw nsw i64 %12, 1
  %16 = icmp eq i64 %15, 48
  br i1 %16, label %18, label %11

; <label>:17:                                     ; preds = %18
  tail call void @free(i8* nonnull %1) #3
  ret i32 0

; <label>:18:                                     ; preds = %11, %18
  %19 = phi i64 [ %23, %18 ], [ 0, %11 ]
  %20 = getelementptr inbounds %struct.packet*, %struct.packet** %2, i64 %19
  %21 = bitcast %struct.packet** %20 to i8**
  %22 = load i8*, i8** %21, align 8, !tbaa !12
  tail call void @free(i8* %22) #3
  %23 = add nuw nsw i64 %19, 1
  %24 = icmp eq i64 %23, 48
  br i1 %24, label %17, label %18
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
!8 = !{!9, !5, i64 0}
!9 = !{!"packet", !5, i64 0, !10, i64 8}
!10 = !{!"long", !6, i64 0}
!11 = !{!9, !10, i64 8}
!12 = !{!13, !13, i64 0}
!13 = !{!"any pointer", !6, i64 0}
