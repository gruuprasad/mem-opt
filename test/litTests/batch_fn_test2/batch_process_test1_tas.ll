; ModuleID = '<stdin>'
source_filename = "batch_process_test1.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [10 x i8] c"batch_arg\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [22 x i8] c"batch_process_test1.c\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [16 x i8] c"tas_batch_maker\00", section "llvm.metadata"
@llvm.global.annotations = appending global [1 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (i32 (i32, i32*, i32*)* @singlularFn to i8*), i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.1, i32 0, i32 0), i32 4 }], section "llvm.metadata"

; Function Attrs: nounwind uwtable
define dso_local i32 @singlularFn(i32 %c, i32* %a, i32* %b) #0 {
entry:
  %a.addr = alloca i32*, align 8
  %b.addr = alloca i32*, align 8
  store i32* %a, i32** %a.addr, align 8, !tbaa !2
  %a.addr1 = bitcast i32** %a.addr to i8*
  call void @llvm.var.annotation(i8* %a.addr1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.1, i32 0, i32 0), i32 4)
  store i32* %b, i32** %b.addr, align 8, !tbaa !2
  %b.addr2 = bitcast i32** %b.addr to i8*
  call void @llvm.var.annotation(i8* %b.addr2, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.1, i32 0, i32 0), i32 4)
  %0 = load i32*, i32** %a.addr, align 8, !tbaa !2
  %1 = load i32, i32* %0, align 4, !tbaa !6
  %cmp = icmp eq i32 %1, 10
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  br label %slowpath

if.end:                                           ; preds = %entry
  %2 = load i32*, i32** %b.addr, align 8, !tbaa !2
  %3 = load i32, i32* %2, align 4, !tbaa !6
  %cmp3 = icmp sgt i32 %3, 10
  br i1 %cmp3, label %if.then4, label %if.else

if.then4:                                         ; preds = %if.end
  %4 = load i32*, i32** %b.addr, align 8, !tbaa !2
  %5 = load i32, i32* %4, align 4, !tbaa !6
  %add = add nsw i32 %5, %c
  %add5 = add nsw i32 %add, 10
  %6 = load i32*, i32** %b.addr, align 8, !tbaa !2
  store i32 %add5, i32* %6, align 4, !tbaa !6
  br label %unlock

if.else:                                          ; preds = %if.end
  %7 = load i32*, i32** %b.addr, align 8, !tbaa !2
  %8 = load i32, i32* %7, align 4, !tbaa !6
  %add6 = add nsw i32 %8, %c
  %add7 = add nsw i32 %add6, 20
  %9 = load i32*, i32** %b.addr, align 8, !tbaa !2
  store i32 %add7, i32* %9, align 4, !tbaa !6
  br label %if.end8

if.end8:                                          ; preds = %if.else
  %10 = load i32*, i32** %a.addr, align 8, !tbaa !2
  %11 = load i32, i32* %10, align 4, !tbaa !6
  %add9 = add nsw i32 %11, 50
  br label %unlock

unlock:                                           ; preds = %if.end8, %if.then4
  %x.0 = phi i32 [ undef, %if.then4 ], [ %add9, %if.end8 ]
  %add10 = add nsw i32 %x.0, 10
  br label %cleanup

slowpath:                                         ; preds = %if.then
  %add11 = add nsw i32 undef, 30
  br label %cleanup

cleanup:                                          ; preds = %slowpath, %unlock
  %retval.0 = phi i32 [ -1, %slowpath ], [ 0, %unlock ]
  ret i32 %retval.0
}

; Function Attrs: nounwind
declare void @llvm.var.annotation(i8*, i8*, i8*, i32) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #2

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #2

; Function Attrs: nounwind uwtable
define dso_local i32 @batch_fn(i32 %c, i32** %batch_arg_1, i32** %batch_arg_2, i16 %TAS_BATCHSIZE, i32* %TAS_RETURNS) #0 {
entry:
  %0 = alloca i32**
  %a.addr = alloca i32*, align 8
  %b.addr = alloca i32*, align 8
  store i32** %batch_arg_1, i32*** %0, align 8, !tbaa !2
  %1 = load i32**, i32*** %0
  %2 = getelementptr i32*, i32** %1, i64 0
  %3 = alloca i32**
  %a.addr1 = bitcast i32** %2 to i8*
  call void @llvm.var.annotation(i8* %a.addr1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.1, i32 0, i32 0), i32 4)
  store i32** %batch_arg_2, i32*** %3, align 8, !tbaa !2
  %4 = load i32**, i32*** %3
  %5 = getelementptr i32*, i32** %4, i64 0
  %6 = alloca i32*
  store i32* %TAS_RETURNS, i32** %6
  %b.addr2 = bitcast i32** %5 to i8*
  call void @llvm.var.annotation(i8* %b.addr2, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.1, i32 0, i32 0), i32 4)
  br label %tas.loop.3.preheader

BatchBlock_begin:                                 ; preds = %tas.loop.3.header
  %7 = load i32**, i32*** %0
  %8 = getelementptr i32*, i32** %7, i64 %35
  %9 = load i32*, i32** %8, align 8, !tbaa !2
  %10 = load i32, i32* %9, align 4, !tbaa !6
  %cmp = icmp eq i32 %10, 10
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %BatchBlock_begin
  br label %slowpath

if.end:                                           ; preds = %BatchBlock_begin
  %11 = load i32**, i32*** %3
  %12 = getelementptr i32*, i32** %11, i64 %35
  %13 = load i32*, i32** %12, align 8, !tbaa !2
  %14 = load i32, i32* %13, align 4, !tbaa !6
  %cmp3 = icmp sgt i32 %14, 10
  br i1 %cmp3, label %if.then4, label %if.else

if.then4:                                         ; preds = %if.end
  %15 = load i32**, i32*** %3
  %16 = getelementptr i32*, i32** %15, i64 %35
  %17 = load i32*, i32** %16, align 8, !tbaa !2
  %18 = load i32, i32* %17, align 4, !tbaa !6
  %add = add nsw i32 %18, %c
  %add5 = add nsw i32 %add, 10
  %19 = load i32**, i32*** %3
  %20 = getelementptr i32*, i32** %19, i64 %35
  %21 = load i32*, i32** %20, align 8, !tbaa !2
  store i32 %add5, i32* %21, align 4, !tbaa !6
  br label %unlock

if.else:                                          ; preds = %if.end
  %22 = load i32**, i32*** %3
  %23 = getelementptr i32*, i32** %22, i64 %35
  %24 = load i32*, i32** %23, align 8, !tbaa !2
  %25 = load i32, i32* %24, align 4, !tbaa !6
  %add6 = add nsw i32 %25, %c
  %add7 = add nsw i32 %add6, 20
  %26 = load i32**, i32*** %3
  %27 = getelementptr i32*, i32** %26, i64 %35
  %28 = load i32*, i32** %27, align 8, !tbaa !2
  store i32 %add7, i32* %28, align 4, !tbaa !6
  br label %if.end8

if.end8:                                          ; preds = %if.else
  %29 = load i32**, i32*** %0
  %30 = getelementptr i32*, i32** %29, i64 %35
  %31 = load i32*, i32** %30, align 8, !tbaa !2
  %32 = load i32, i32* %31, align 4, !tbaa !6
  %add9 = add nsw i32 %32, 50
  br label %unlock

unlock:                                           ; preds = %if.end8, %if.then4
  %x.0 = phi i32 [ undef, %if.then4 ], [ %add9, %if.end8 ]
  %add10 = add nsw i32 %x.0, 10
  br label %cleanup

slowpath:                                         ; preds = %if.then
  %add11 = add nsw i32 undef, 30
  br label %cleanup

cleanup:                                          ; preds = %slowpath, %unlock
  %retval.0 = phi i32 [ -1, %slowpath ], [ 0, %unlock ]
  %33 = load i32*, i32** %6
  %34 = getelementptr i32, i32* %33, i64 %35
  store i32 %retval.0, i32* %34
  br label %Knotblock

tas.loop.3.preheader:                             ; preds = %entry
  br label %tas.loop.3.header

tas.loop.3.header:                                ; preds = %tas.loop.3.latch, %tas.loop.3.preheader
  %indV = phi i16 [ 0, %tas.loop.3.preheader ], [ %36, %tas.loop.3.latch ]
  %35 = sext i16 %indV to i64
  %loop-predicate = icmp slt i16 %indV, %TAS_BATCHSIZE
  br i1 %loop-predicate, label %BatchBlock_begin, label %EndBlock

tas.loop.3.latch:                                 ; preds = %Knotblock
  %36 = add i16 %indV, 1
  br label %tas.loop.3.header

EndBlock:                                         ; preds = %tas.loop.3.header
  ret i32 0

Knotblock:                                        ; preds = %cleanup
  br label %tas.loop.3.latch
}

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "tas_batch_maker" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { argmemonly nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (trunk 341799)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"any pointer", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = !{!7, !7, i64 0}
!7 = !{!"int", !4, i64 0}
