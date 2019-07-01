; ModuleID = '<stdin>'
source_filename = "batch_process_test1.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [10 x i8] c"batch_arg\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [22 x i8] c"batch_process_test1.c\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [16 x i8] c"tas_batch_maker\00", section "llvm.metadata"
@llvm.global.annotations = appending global [1 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (i32 (i32, i32*, i32*)* @singlularFn to i8*), i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.1, i32 0, i32 0), i32 4 }], section "llvm.metadata"

; Function Attrs: nounwind uwtable
define dso_local i32 @singlularFn(i32 %c, i32* %b, i32* %a) #0 {
entry:
  %b.addr = alloca i32*, align 8
  %a.addr = alloca i32*, align 8
  store i32* %b, i32** %b.addr, align 8, !tbaa !2
  %b.addr1 = bitcast i32** %b.addr to i8*
  call void @llvm.var.annotation(i8* %b.addr1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.1, i32 0, i32 0), i32 4)
  store i32* %a, i32** %a.addr, align 8, !tbaa !2
  %a.addr2 = bitcast i32** %a.addr to i8*
  call void @llvm.var.annotation(i8* %a.addr2, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.1, i32 0, i32 0), i32 4)
  %0 = load i32*, i32** %b.addr, align 8, !tbaa !2
  store i32 10, i32* %0, align 4, !tbaa !6
  %1 = load i32*, i32** %a.addr, align 8, !tbaa !2
  %2 = load i32, i32* %1, align 4, !tbaa !6
  %cmp = icmp sgt i32 %2, 10
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %3 = load i32*, i32** %a.addr, align 8, !tbaa !2
  %4 = load i32, i32* %3, align 4, !tbaa !6
  %add = add nsw i32 %4, %c
  %add3 = add nsw i32 %add, 10
  %5 = load i32*, i32** %a.addr, align 8, !tbaa !2
  store i32 %add3, i32* %5, align 4, !tbaa !6
  br label %if.end

if.else:                                          ; preds = %entry
  %6 = load i32*, i32** %a.addr, align 8, !tbaa !2
  %7 = load i32, i32* %6, align 4, !tbaa !6
  %add4 = add nsw i32 %7, %c
  %add5 = add nsw i32 %add4, 20
  %8 = load i32*, i32** %a.addr, align 8, !tbaa !2
  store i32 %add5, i32* %8, align 4, !tbaa !6
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  ret i32 0
}

; Function Attrs: nounwind
declare void @llvm.var.annotation(i8*, i8*, i8*, i32) #1

; Function Attrs: nounwind uwtable
define dso_local i32 @batchRefFn(i32 %c, i32** %b, i32** %a, i32 %n) #2 {
entry:
  %0 = sext i32 %n to i64
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %indvars.iv = phi i64 [ %indvars.iv.next, %for.inc ], [ 0, %entry ]
  %cmp = icmp slt i64 %indvars.iv, %0
  br i1 %cmp, label %for.body, label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %for.cond
  br label %for.end

for.body:                                         ; preds = %for.cond
  %arrayidx = getelementptr inbounds i32*, i32** %a, i64 %indvars.iv
  %1 = load i32*, i32** %arrayidx, align 8, !tbaa !2
  %2 = load i32, i32* %1, align 4, !tbaa !6
  %cmp1 = icmp sgt i32 %2, 10
  br i1 %cmp1, label %if.then, label %if.else

if.then:                                          ; preds = %for.body
  %arrayidx3 = getelementptr inbounds i32*, i32** %a, i64 %indvars.iv
  %3 = load i32*, i32** %arrayidx3, align 8, !tbaa !2
  %4 = load i32, i32* %3, align 4, !tbaa !6
  %add = add nsw i32 %4, %c
  %add4 = add nsw i32 %add, 10
  %arrayidx6 = getelementptr inbounds i32*, i32** %a, i64 %indvars.iv
  %5 = load i32*, i32** %arrayidx6, align 8, !tbaa !2
  store i32 %add4, i32* %5, align 4, !tbaa !6
  br label %if.end

if.else:                                          ; preds = %for.body
  %arrayidx8 = getelementptr inbounds i32*, i32** %a, i64 %indvars.iv
  %6 = load i32*, i32** %arrayidx8, align 8, !tbaa !2
  %7 = load i32, i32* %6, align 4, !tbaa !6
  %add9 = add nsw i32 %7, %c
  %add10 = add nsw i32 %add9, 20
  %arrayidx12 = getelementptr inbounds i32*, i32** %a, i64 %indvars.iv
  %8 = load i32*, i32** %arrayidx12, align 8, !tbaa !2
  store i32 %add10, i32* %8, align 4, !tbaa !6
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond.cleanup
  ret i32 0
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #3

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #3

; Function Attrs: nounwind uwtable
define dso_local i32 @batch_fn(i32 %c, i32** %batch_arg_1, i32** %batch_arg_2, i16 %TAS_BATCHSIZE) #0 {
entry:
  %0 = alloca i32**
  %b.addr = alloca i32*, align 8
  %a.addr = alloca i32*, align 8
  store i32** %batch_arg_1, i32*** %0, align 8, !tbaa !2
  %1 = load i32**, i32*** %0
  %2 = getelementptr i32*, i32** %1, i64 0
  %3 = alloca i32**
  %b.addr1 = bitcast i32** %2 to i8*
  call void @llvm.var.annotation(i8* %b.addr1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.1, i32 0, i32 0), i32 4)
  store i32** %batch_arg_2, i32*** %3, align 8, !tbaa !2
  %4 = load i32**, i32*** %3
  %5 = getelementptr i32*, i32** %4, i64 0
  %a.addr2 = bitcast i32** %5 to i8*
  call void @llvm.var.annotation(i8* %a.addr2, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.1, i32 0, i32 0), i32 4)
  %6 = alloca [32 x i32]
  br label %tas.loop.3.preheader

BatchBlock_begin:                                 ; preds = %tas.loop.3.header
  %7 = load i32**, i32*** %0
  %8 = getelementptr i32*, i32** %7, i64 %29
  %9 = load i32*, i32** %8, align 8, !tbaa !2
  store i32 10, i32* %9, align 4, !tbaa !6
  %10 = load i32**, i32*** %3
  %11 = getelementptr i32*, i32** %10, i64 %29
  %12 = load i32*, i32** %11, align 8, !tbaa !2
  %13 = load i32, i32* %12, align 4, !tbaa !6
  %cmp = icmp sgt i32 %13, 10
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %BatchBlock_begin
  %14 = load i32**, i32*** %3
  %15 = getelementptr i32*, i32** %14, i64 %29
  %16 = load i32*, i32** %15, align 8, !tbaa !2
  %17 = load i32, i32* %16, align 4, !tbaa !6
  %add = add nsw i32 %17, %c
  %add3 = add nsw i32 %add, 10
  %18 = load i32**, i32*** %3
  %19 = getelementptr i32*, i32** %18, i64 %29
  %20 = load i32*, i32** %19, align 8, !tbaa !2
  store i32 %add3, i32* %20, align 4, !tbaa !6
  br label %if.end

if.else:                                          ; preds = %BatchBlock_begin
  %21 = load i32**, i32*** %3
  %22 = getelementptr i32*, i32** %21, i64 %29
  %23 = load i32*, i32** %22, align 8, !tbaa !2
  %24 = load i32, i32* %23, align 4, !tbaa !6
  %add4 = add nsw i32 %24, %c
  %add5 = add nsw i32 %add4, 20
  %25 = load i32**, i32*** %3
  %26 = getelementptr i32*, i32** %25, i64 %29
  %27 = load i32*, i32** %26, align 8, !tbaa !2
  store i32 %add5, i32* %27, align 4, !tbaa !6
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %28 = getelementptr [32 x i32], [32 x i32]* %6, i64 0, i16 %indV
  store i32 0, i32* %28
  br label %Knotblock

tas.loop.3.preheader:                             ; preds = %entry
  br label %tas.loop.3.header

tas.loop.3.header:                                ; preds = %tas.loop.3.latch, %tas.loop.3.preheader
  %indV = phi i16 [ 0, %tas.loop.3.preheader ], [ %30, %tas.loop.3.latch ]
  %29 = sext i16 %indV to i64
  %loop-predicate = icmp slt i16 %indV, %TAS_BATCHSIZE
  br i1 %loop-predicate, label %BatchBlock_begin, label %EndBlock

tas.loop.3.latch:                                 ; preds = %Knotblock
  %30 = add i16 %indV, 1
  br label %tas.loop.3.header

EndBlock:                                         ; preds = %tas.loop.3.header
  ret i32 0

Knotblock:                                        ; preds = %if.end
  br label %tas.loop.3.latch
}

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "tas_batch_maker" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { argmemonly nounwind }

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
