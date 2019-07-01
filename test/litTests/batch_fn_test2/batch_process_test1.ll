; ModuleID = 'batch_process_test1.c'
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
  %retval = alloca i32, align 4
  %c.addr = alloca i32, align 4
  %a.addr = alloca i32*, align 8
  %b.addr = alloca i32*, align 8
  %x = alloca i32, align 4
  %cleanup.dest.slot = alloca i32, align 4
  store i32 %c, i32* %c.addr, align 4, !tbaa !2
  store i32* %a, i32** %a.addr, align 8, !tbaa !6
  %a.addr1 = bitcast i32** %a.addr to i8*
  call void @llvm.var.annotation(i8* %a.addr1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.1, i32 0, i32 0), i32 4)
  store i32* %b, i32** %b.addr, align 8, !tbaa !6
  %b.addr2 = bitcast i32** %b.addr to i8*
  call void @llvm.var.annotation(i8* %b.addr2, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.1, i32 0, i32 0), i32 4)
  %0 = bitcast i32* %x to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* %0) #1
  %1 = load i32*, i32** %a.addr, align 8, !tbaa !6
  %2 = load i32, i32* %1, align 4, !tbaa !2
  %cmp = icmp eq i32 %2, 10
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  br label %slowpath

if.end:                                           ; preds = %entry
  %3 = load i32*, i32** %b.addr, align 8, !tbaa !6
  %4 = load i32, i32* %3, align 4, !tbaa !2
  %cmp3 = icmp sgt i32 %4, 10
  br i1 %cmp3, label %if.then4, label %if.else

if.then4:                                         ; preds = %if.end
  %5 = load i32*, i32** %b.addr, align 8, !tbaa !6
  %6 = load i32, i32* %5, align 4, !tbaa !2
  %7 = load i32, i32* %c.addr, align 4, !tbaa !2
  %add = add nsw i32 %6, %7
  %add5 = add nsw i32 %add, 10
  %8 = load i32*, i32** %b.addr, align 8, !tbaa !6
  store i32 %add5, i32* %8, align 4, !tbaa !2
  br label %unlock

if.else:                                          ; preds = %if.end
  %9 = load i32*, i32** %b.addr, align 8, !tbaa !6
  %10 = load i32, i32* %9, align 4, !tbaa !2
  %11 = load i32, i32* %c.addr, align 4, !tbaa !2
  %add6 = add nsw i32 %10, %11
  %add7 = add nsw i32 %add6, 20
  %12 = load i32*, i32** %b.addr, align 8, !tbaa !6
  store i32 %add7, i32* %12, align 4, !tbaa !2
  br label %if.end8

if.end8:                                          ; preds = %if.else
  %13 = load i32*, i32** %a.addr, align 8, !tbaa !6
  %14 = load i32, i32* %13, align 4, !tbaa !2
  %add9 = add nsw i32 %14, 50
  store i32 %add9, i32* %x, align 4, !tbaa !2
  br label %unlock

unlock:                                           ; preds = %if.end8, %if.then4
  %15 = load i32, i32* %x, align 4, !tbaa !2
  %add10 = add nsw i32 %15, 10
  store i32 %add10, i32* %x, align 4, !tbaa !2
  store i32 0, i32* %retval, align 4
  store i32 1, i32* %cleanup.dest.slot, align 4
  br label %cleanup

slowpath:                                         ; preds = %if.then
  %16 = load i32, i32* %x, align 4, !tbaa !2
  %add11 = add nsw i32 %16, 30
  store i32 %add11, i32* %x, align 4, !tbaa !2
  store i32 -1, i32* %retval, align 4
  store i32 1, i32* %cleanup.dest.slot, align 4
  br label %cleanup

cleanup:                                          ; preds = %slowpath, %unlock
  %17 = bitcast i32* %x to i8*
  call void @llvm.lifetime.end.p0i8(i64 4, i8* %17) #1
  %18 = load i32, i32* %retval, align 4
  ret i32 %18
}

; Function Attrs: nounwind
declare void @llvm.var.annotation(i8*, i8*, i8*, i32) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #2

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #2

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { argmemonly nounwind }

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
