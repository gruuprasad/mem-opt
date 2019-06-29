; ModuleID = 'batch_process_test1.c'
source_filename = "batch_process_test1.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.1 = private unnamed_addr constant [10 x i8] c"batch_arg\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [22 x i8] c"batch_process_test1.c\00", section "llvm.metadata"
@.str.3 = private unnamed_addr constant [16 x i8] c"tas_batch_maker\00", section "llvm.metadata"
@llvm.global.annotations = appending global [1 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (i32 (i32, i32*, i32*)* @singlularFn to i8*), i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.2, i32 0, i32 0), i32 53 }], section "llvm.metadata"

; Function Attrs: nounwind uwtable
define dso_local i32 @RefBatchFn(i32 %d, i32** %b, i32** %a) #0 {
entry:
  %d.addr = alloca i32, align 4
  %b.addr = alloca i32**, align 8
  %a.addr = alloca i32**, align 8
  %c = alloca i32, align 4
  %i = alloca i32, align 4
  %i9 = alloca i32, align 4
  store i32 %d, i32* %d.addr, align 4, !tbaa !2
  store i32** %b, i32*** %b.addr, align 8, !tbaa !6
  store i32** %a, i32*** %a.addr, align 8, !tbaa !6
  %0 = load i32, i32* %d.addr, align 4, !tbaa !2
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %0)
  %1 = bitcast i32* %c to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* %1) #3
  store i32 10, i32* %c, align 4, !tbaa !2
  %2 = load i32**, i32*** %a.addr, align 8, !tbaa !6
  %arrayidx = getelementptr inbounds i32*, i32** %2, i64 0
  %3 = load i32*, i32** %arrayidx, align 8, !tbaa !6
  %4 = load i32, i32* %3, align 4, !tbaa !2
  %cmp = icmp sgt i32 %4, 10
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %5 = load i32**, i32*** %a.addr, align 8, !tbaa !6
  %arrayidx1 = getelementptr inbounds i32*, i32** %5, i64 0
  %6 = load i32*, i32** %arrayidx1, align 8, !tbaa !6
  %7 = load i32, i32* %6, align 4, !tbaa !2
  %add = add nsw i32 %7, 10
  %8 = load i32**, i32*** %a.addr, align 8, !tbaa !6
  %arrayidx2 = getelementptr inbounds i32*, i32** %8, i64 0
  %9 = load i32*, i32** %arrayidx2, align 8, !tbaa !6
  store i32 %add, i32* %9, align 4, !tbaa !2
  br label %if.end

if.else:                                          ; preds = %entry
  %10 = load i32**, i32*** %a.addr, align 8, !tbaa !6
  %arrayidx3 = getelementptr inbounds i32*, i32** %10, i64 1
  %11 = load i32*, i32** %arrayidx3, align 8, !tbaa !6
  %12 = load i32, i32* %11, align 4, !tbaa !2
  %add4 = add nsw i32 %12, 20
  %13 = load i32**, i32*** %a.addr, align 8, !tbaa !6
  %arrayidx5 = getelementptr inbounds i32*, i32** %13, i64 1
  %14 = load i32*, i32** %arrayidx5, align 8, !tbaa !6
  store i32 %add4, i32* %14, align 4, !tbaa !2
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %15 = bitcast i32* %i to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* %15) #3
  store i32 0, i32* %i, align 4, !tbaa !2
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.end
  %16 = load i32, i32* %i, align 4, !tbaa !2
  %cmp6 = icmp slt i32 %16, 2
  br i1 %cmp6, label %for.body, label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %for.cond
  %17 = bitcast i32* %i to i8*
  call void @llvm.lifetime.end.p0i8(i64 4, i8* %17) #3
  br label %for.end

for.body:                                         ; preds = %for.cond
  %18 = load i32**, i32*** %a.addr, align 8, !tbaa !6
  %19 = load i32, i32* %i, align 4, !tbaa !2
  %idxprom = sext i32 %19 to i64
  %arrayidx7 = getelementptr inbounds i32*, i32** %18, i64 %idxprom
  %20 = load i32*, i32** %arrayidx7, align 8, !tbaa !6
  %21 = load i32, i32* %20, align 4, !tbaa !2
  %call8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %21)
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %22 = load i32, i32* %i, align 4, !tbaa !2
  %inc = add nsw i32 %22, 1
  store i32 %inc, i32* %i, align 4, !tbaa !2
  br label %for.cond

for.end:                                          ; preds = %for.cond.cleanup
  %23 = bitcast i32* %i9 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* %23) #3
  store i32 0, i32* %i9, align 4, !tbaa !2
  br label %for.cond10

for.cond10:                                       ; preds = %for.inc17, %for.end
  %24 = load i32, i32* %i9, align 4, !tbaa !2
  %cmp11 = icmp slt i32 %24, 2
  br i1 %cmp11, label %for.body13, label %for.cond.cleanup12

for.cond.cleanup12:                               ; preds = %for.cond10
  %25 = bitcast i32* %i9 to i8*
  call void @llvm.lifetime.end.p0i8(i64 4, i8* %25) #3
  br label %for.end19

for.body13:                                       ; preds = %for.cond10
  %26 = load i32**, i32*** %b.addr, align 8, !tbaa !6
  %27 = load i32, i32* %i9, align 4, !tbaa !2
  %idxprom14 = sext i32 %27 to i64
  %arrayidx15 = getelementptr inbounds i32*, i32** %26, i64 %idxprom14
  %28 = load i32*, i32** %arrayidx15, align 8, !tbaa !6
  %29 = load i32, i32* %28, align 4, !tbaa !2
  %call16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %29)
  br label %for.inc17

for.inc17:                                        ; preds = %for.body13
  %30 = load i32, i32* %i9, align 4, !tbaa !2
  %inc18 = add nsw i32 %30, 1
  store i32 %inc18, i32* %i9, align 4, !tbaa !2
  br label %for.cond10

for.end19:                                        ; preds = %for.cond.cleanup12
  %31 = load i32, i32* %d.addr, align 4, !tbaa !2
  %add20 = add nsw i32 %31, 10
  store i32 %add20, i32* %d.addr, align 4, !tbaa !2
  %32 = bitcast i32* %c to i8*
  call void @llvm.lifetime.end.p0i8(i64 4, i8* %32) #3
  ret i32 0
}

declare dso_local i32 @printf(i8*, ...) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #2

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #2

; Function Attrs: nounwind uwtable
define dso_local i32 @batchFn1(i32 %c, i32** %b, i32** %a) #0 {
entry:
  %c.addr = alloca i32, align 4
  %b.addr = alloca i32**, align 8
  %a.addr = alloca i32**, align 8
  store i32 %c, i32* %c.addr, align 4, !tbaa !2
  store i32** %b, i32*** %b.addr, align 8, !tbaa !6
  store i32** %a, i32*** %a.addr, align 8, !tbaa !6
  %0 = load i32, i32* %c.addr, align 4, !tbaa !2
  %add = add nsw i32 %0, 10
  store i32 %add, i32* %c.addr, align 4, !tbaa !2
  %1 = load i32**, i32*** %b.addr, align 8, !tbaa !6
  %arrayidx = getelementptr inbounds i32*, i32** %1, i64 0
  %2 = load i32*, i32** %arrayidx, align 8, !tbaa !6
  store i32 10, i32* %2, align 4, !tbaa !2
  %3 = load i32**, i32*** %a.addr, align 8, !tbaa !6
  %arrayidx1 = getelementptr inbounds i32*, i32** %3, i64 0
  %4 = load i32*, i32** %arrayidx1, align 8, !tbaa !6
  %5 = load i32, i32* %4, align 4, !tbaa !2
  %cmp = icmp sgt i32 %5, 10
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %6 = load i32**, i32*** %a.addr, align 8, !tbaa !6
  %arrayidx2 = getelementptr inbounds i32*, i32** %6, i64 0
  %7 = load i32*, i32** %arrayidx2, align 8, !tbaa !6
  %8 = load i32, i32* %7, align 4, !tbaa !2
  %add3 = add nsw i32 %8, 10
  %9 = load i32**, i32*** %a.addr, align 8, !tbaa !6
  %arrayidx4 = getelementptr inbounds i32*, i32** %9, i64 0
  %10 = load i32*, i32** %arrayidx4, align 8, !tbaa !6
  store i32 %add3, i32* %10, align 4, !tbaa !2
  br label %if.end

if.else:                                          ; preds = %entry
  %11 = load i32**, i32*** %a.addr, align 8, !tbaa !6
  %arrayidx5 = getelementptr inbounds i32*, i32** %11, i64 0
  %12 = load i32*, i32** %arrayidx5, align 8, !tbaa !6
  %13 = load i32, i32* %12, align 4, !tbaa !2
  %add6 = add nsw i32 %13, 20
  %14 = load i32**, i32*** %a.addr, align 8, !tbaa !6
  %arrayidx7 = getelementptr inbounds i32*, i32** %14, i64 0
  %15 = load i32*, i32** %arrayidx7, align 8, !tbaa !6
  store i32 %add6, i32* %15, align 4, !tbaa !2
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %16 = load i32, i32* %c.addr, align 4, !tbaa !2
  %add8 = add nsw i32 %16, 20
  store i32 %add8, i32* %c.addr, align 4, !tbaa !2
  ret i32 0
}

; Function Attrs: nounwind uwtable
define dso_local i32 @singlularFn(i32 %c, i32* %b, i32* %a) #0 {
entry:
  %c.addr = alloca i32, align 4
  %b.addr = alloca i32*, align 8
  %a.addr = alloca i32*, align 8
  store i32 %c, i32* %c.addr, align 4, !tbaa !2
  store i32* %b, i32** %b.addr, align 8, !tbaa !6
  %b.addr1 = bitcast i32** %b.addr to i8*
  call void @llvm.var.annotation(i8* %b.addr1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.2, i32 0, i32 0), i32 53)
  store i32* %a, i32** %a.addr, align 8, !tbaa !6
  %a.addr2 = bitcast i32** %a.addr to i8*
  call void @llvm.var.annotation(i8* %a.addr2, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.2, i32 0, i32 0), i32 53)
  %0 = load i32*, i32** %b.addr, align 8, !tbaa !6
  store i32 10, i32* %0, align 4, !tbaa !2
  %1 = load i32*, i32** %a.addr, align 8, !tbaa !6
  %2 = load i32, i32* %1, align 4, !tbaa !2
  %cmp = icmp sgt i32 %2, 10
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %3 = load i32*, i32** %a.addr, align 8, !tbaa !6
  %4 = load i32, i32* %3, align 4, !tbaa !2
  %add = add nsw i32 %4, 10
  %5 = load i32*, i32** %a.addr, align 8, !tbaa !6
  store i32 %add, i32* %5, align 4, !tbaa !2
  %6 = load i32, i32* %c.addr, align 4, !tbaa !2
  %add3 = add nsw i32 %6, 10
  store i32 %add3, i32* %c.addr, align 4, !tbaa !2
  br label %if.end

if.else:                                          ; preds = %entry
  %7 = load i32*, i32** %a.addr, align 8, !tbaa !6
  %8 = load i32, i32* %7, align 4, !tbaa !2
  %add4 = add nsw i32 %8, 20
  %9 = load i32*, i32** %a.addr, align 8, !tbaa !6
  store i32 %add4, i32* %9, align 4, !tbaa !2
  %10 = load i32, i32* %c.addr, align 4, !tbaa !2
  %add5 = add nsw i32 %10, 20
  store i32 %add5, i32* %c.addr, align 4, !tbaa !2
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %11 = load i32, i32* %c.addr, align 4, !tbaa !2
  ret i32 %11
}

; Function Attrs: nounwind
declare void @llvm.var.annotation(i8*, i8*, i8*, i32) #3

; Function Attrs: nounwind uwtable
define dso_local i32 @test() #0 {
entry:
  %a1 = alloca i32, align 4
  %a2 = alloca i32, align 4
  %b1 = alloca i32, align 4
  %b2 = alloca i32, align 4
  %a_list = alloca [2 x i32*], align 16
  %b_list = alloca [2 x i32*], align 16
  %0 = bitcast i32* %a1 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* %0) #3
  store i32 10, i32* %a1, align 4, !tbaa !2
  %1 = bitcast i32* %a2 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* %1) #3
  store i32 20, i32* %a2, align 4, !tbaa !2
  %2 = bitcast i32* %b1 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* %2) #3
  store i32 30, i32* %b1, align 4, !tbaa !2
  %3 = bitcast i32* %b2 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* %3) #3
  store i32 40, i32* %b2, align 4, !tbaa !2
  %4 = bitcast [2 x i32*]* %a_list to i8*
  call void @llvm.lifetime.start.p0i8(i64 16, i8* %4) #3
  %arrayinit.begin = getelementptr inbounds [2 x i32*], [2 x i32*]* %a_list, i64 0, i64 0
  store i32* %a1, i32** %arrayinit.begin, align 8, !tbaa !6
  %arrayinit.element = getelementptr inbounds i32*, i32** %arrayinit.begin, i64 1
  store i32* %a2, i32** %arrayinit.element, align 8, !tbaa !6
  %5 = bitcast [2 x i32*]* %b_list to i8*
  call void @llvm.lifetime.start.p0i8(i64 16, i8* %5) #3
  %arrayinit.begin1 = getelementptr inbounds [2 x i32*], [2 x i32*]* %b_list, i64 0, i64 0
  store i32* %b1, i32** %arrayinit.begin1, align 8, !tbaa !6
  %arrayinit.element2 = getelementptr inbounds i32*, i32** %arrayinit.begin1, i64 1
  store i32* %b2, i32** %arrayinit.element2, align 8, !tbaa !6
  %arraydecay = getelementptr inbounds [2 x i32*], [2 x i32*]* %a_list, i32 0, i32 0
  %arraydecay3 = getelementptr inbounds [2 x i32*], [2 x i32*]* %b_list, i32 0, i32 0
  %call = call i32 @RefBatchFn(i32 50, i32** %arraydecay, i32** %arraydecay3)
  %6 = bitcast [2 x i32*]* %b_list to i8*
  call void @llvm.lifetime.end.p0i8(i64 16, i8* %6) #3
  %7 = bitcast [2 x i32*]* %a_list to i8*
  call void @llvm.lifetime.end.p0i8(i64 16, i8* %7) #3
  %8 = bitcast i32* %b2 to i8*
  call void @llvm.lifetime.end.p0i8(i64 4, i8* %8) #3
  %9 = bitcast i32* %b1 to i8*
  call void @llvm.lifetime.end.p0i8(i64 4, i8* %9) #3
  %10 = bitcast i32* %a2 to i8*
  call void @llvm.lifetime.end.p0i8(i64 4, i8* %10) #3
  %11 = bitcast i32* %a1 to i8*
  call void @llvm.lifetime.end.p0i8(i64 4, i8* %11) #3
  ret i32 0
}

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { argmemonly nounwind }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (trunk 341799)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = !{!7, !7, i64 0}
!7 = !{!"any pointer", !4, i64 0}
