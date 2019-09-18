; ModuleID = 'batchmaker_test2.c'
source_filename = "batchmaker_test2.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.packet = type { i32, i32, i32, i32 }

@.str = private unnamed_addr constant [10 x i8] c"batch_arg\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [19 x i8] c"batchmaker_test2.c\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [16 x i8] c"packet ip = %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [16 x i8] c"packet id = %d\0A\00", align 1
@.str.4 = private unnamed_addr constant [9 x i8] c"fastpath\00", align 1
@.str.5 = private unnamed_addr constant [9 x i8] c"slowpath\00", align 1
@.str.6 = private unnamed_addr constant [7 x i8] c"unlock\00", align 1
@.str.7 = private unnamed_addr constant [16 x i8] c"tas_batch_maker\00", section "llvm.metadata"
@.str.8 = private unnamed_addr constant [7 x i8] c"x = %d\00", align 1
@llvm.global.annotations = appending global [1 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (i32 (%struct.packet*)* @process_packet to i8*), i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.7, i32 0, i32 0), i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i32 0, i32 0), i32 17 }], section "llvm.metadata"

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @process_packet(%struct.packet*) #0 {
  %2 = alloca i32, align 4
  %3 = alloca %struct.packet*, align 8
  %4 = alloca %struct.packet*, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store %struct.packet* %0, %struct.packet** %3, align 8
  %8 = bitcast %struct.packet** %3 to i8*
  call void @llvm.var.annotation(i8* %8, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i32 0, i32 0), i32 17)
  %9 = load %struct.packet*, %struct.packet** %3, align 8
  store %struct.packet* %9, %struct.packet** %4, align 8
  store i32 0, i32* %5, align 4
  store i32 0, i32* %6, align 4
  store i32 0, i32* %7, align 4
  %10 = load %struct.packet*, %struct.packet** %4, align 8
  %11 = getelementptr inbounds %struct.packet, %struct.packet* %10, i32 0, i32 0
  %12 = load i32, i32* %11, align 4
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2, i32 0, i32 0), i32 %12)
  %14 = load %struct.packet*, %struct.packet** %4, align 8
  %15 = getelementptr inbounds %struct.packet, %struct.packet* %14, i32 0, i32 1
  %16 = load i32, i32* %15, align 4
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.3, i32 0, i32 0), i32 %16)
  %18 = load %struct.packet*, %struct.packet** %4, align 8
  %19 = getelementptr inbounds %struct.packet, %struct.packet* %18, i32 0, i32 1
  %20 = load i32, i32* %19, align 4
  %21 = icmp eq i32 %20, 1
  br i1 %21, label %22, label %23

; <label>:22:                                     ; preds = %1
  br label %58

; <label>:23:                                     ; preds = %1
  %24 = load %struct.packet*, %struct.packet** %4, align 8
  %25 = getelementptr inbounds %struct.packet, %struct.packet* %24, i32 0, i32 1
  %26 = load i32, i32* %25, align 4
  %27 = icmp eq i32 %26, 2
  br i1 %27, label %28, label %29

; <label>:28:                                     ; preds = %23
  br label %56

; <label>:29:                                     ; preds = %23
  %30 = load i32, i32* %5, align 4
  %31 = icmp eq i32 %30, 0
  br i1 %31, label %32, label %41

; <label>:32:                                     ; preds = %29
  %33 = load i32, i32* %5, align 4
  %34 = add nsw i32 %33, 100
  store i32 %34, i32* %5, align 4
  %35 = load i32, i32* %6, align 4
  %36 = add nsw i32 %35, 200
  store i32 %36, i32* %6, align 4
  %37 = load i32, i32* %5, align 4
  %38 = load i32, i32* %6, align 4
  %39 = add nsw i32 %37, %38
  %40 = add nsw i32 %39, 50
  store i32 %40, i32* %7, align 4
  br label %56

; <label>:41:                                     ; preds = %29
  %42 = load i32, i32* %5, align 4
  %43 = icmp sgt i32 %42, 50
  br i1 %43, label %44, label %49

; <label>:44:                                     ; preds = %41
  %45 = load i32, i32* %5, align 4
  %46 = sub nsw i32 %45, 50
  store i32 %46, i32* %5, align 4
  %47 = load i32, i32* %6, align 4
  %48 = sub nsw i32 %47, 100
  store i32 %48, i32* %6, align 4
  br label %54

; <label>:49:                                     ; preds = %41
  %50 = load i32, i32* %5, align 4
  %51 = load i32, i32* %6, align 4
  %52 = add nsw i32 %50, %51
  %53 = add nsw i32 %52, 50
  store i32 %53, i32* %7, align 4
  br label %54

; <label>:54:                                     ; preds = %49, %44
  %55 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.4, i32 0, i32 0))
  store i32 0, i32* %2, align 4
  br label %60

; <label>:56:                                     ; preds = %32, %28
  %57 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.5, i32 0, i32 0))
  store i32 -2, i32* %2, align 4
  br label %60

; <label>:58:                                     ; preds = %22
  %59 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.6, i32 0, i32 0))
  store i32 -1, i32* %2, align 4
  br label %60

; <label>:60:                                     ; preds = %58, %56, %54
  %61 = load i32, i32* %2, align 4
  ret i32 %61
}

; Function Attrs: nounwind
declare void @llvm.var.annotation(i8*, i8*, i8*, i32) #1

declare i32 @printf(i8*, ...) #2

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @process_packet_batch(%struct.packet**) #0 {
  %2 = alloca %struct.packet**, align 8
  %3 = alloca i32, align 4
  %4 = alloca %struct.packet*, align 8
  %5 = alloca i32, align 4
  %6 = alloca [1 x i32], align 4
  %7 = alloca i32, align 4
  store %struct.packet** %0, %struct.packet*** %2, align 8
  store i32 0, i32* %3, align 4
  store i32 0, i32* %5, align 4
  %8 = getelementptr inbounds [1 x i32], [1 x i32]* %6, i64 0, i64 0
  store i32 0, i32* %8, align 4
  store i32 0, i32* %7, align 4
  %9 = load %struct.packet**, %struct.packet*** %2, align 8
  %10 = load i32, i32* %5, align 4
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds %struct.packet*, %struct.packet** %9, i64 %11
  %13 = load %struct.packet*, %struct.packet** %12, align 8
  store %struct.packet* %13, %struct.packet** %4, align 8
  %14 = load %struct.packet*, %struct.packet** %4, align 8
  %15 = getelementptr inbounds %struct.packet, %struct.packet* %14, i32 0, i32 0
  %16 = load i32, i32* %15, align 4
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2, i32 0, i32 0), i32 %16)
  %18 = load %struct.packet*, %struct.packet** %4, align 8
  %19 = getelementptr inbounds %struct.packet, %struct.packet* %18, i32 0, i32 1
  %20 = load i32, i32* %19, align 4
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.3, i32 0, i32 0), i32 %20)
  %22 = load i32, i32* %7, align 4
  %23 = icmp eq i32 %22, 0
  br i1 %23, label %24, label %32

; <label>:24:                                     ; preds = %1
  %25 = load %struct.packet*, %struct.packet** %4, align 8
  %26 = getelementptr inbounds %struct.packet, %struct.packet* %25, i32 0, i32 1
  %27 = load i32, i32* %26, align 4
  %28 = icmp eq i32 %27, 1
  br i1 %28, label %29, label %32

; <label>:29:                                     ; preds = %24
  %30 = load i32, i32* %3, align 4
  %31 = add nsw i32 %30, 10
  store i32 %31, i32* %3, align 4
  store i32 1, i32* %7, align 4
  br label %32

; <label>:32:                                     ; preds = %29, %24, %1
  %33 = load i32, i32* %7, align 4
  %34 = icmp eq i32 %33, 0
  br i1 %34, label %35, label %43

; <label>:35:                                     ; preds = %32
  %36 = load %struct.packet*, %struct.packet** %4, align 8
  %37 = getelementptr inbounds %struct.packet, %struct.packet* %36, i32 0, i32 1
  %38 = load i32, i32* %37, align 4
  %39 = icmp eq i32 %38, 2
  br i1 %39, label %40, label %43

; <label>:40:                                     ; preds = %35
  %41 = load i32, i32* %3, align 4
  %42 = add nsw i32 %41, 20
  store i32 %42, i32* %3, align 4
  store i32 2, i32* %7, align 4
  br label %43

; <label>:43:                                     ; preds = %40, %35, %32
  %44 = load i32, i32* %7, align 4
  %45 = icmp eq i32 %44, 0
  br i1 %45, label %46, label %51

; <label>:46:                                     ; preds = %43
  %47 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.4, i32 0, i32 0))
  store i32 3, i32* %7, align 4
  %48 = load i32, i32* %5, align 4
  %49 = sext i32 %48 to i64
  %50 = getelementptr inbounds [1 x i32], [1 x i32]* %6, i64 0, i64 %49
  store i32 0, i32* %50, align 4
  br label %51

; <label>:51:                                     ; preds = %46, %43
  br label %52

; <label>:52:                                     ; preds = %51
  %53 = load i32, i32* %7, align 4
  %54 = icmp eq i32 %53, 1
  br i1 %54, label %55, label %60

; <label>:55:                                     ; preds = %52
  %56 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.6, i32 0, i32 0))
  %57 = load i32, i32* %5, align 4
  %58 = sext i32 %57 to i64
  %59 = getelementptr inbounds [1 x i32], [1 x i32]* %6, i64 0, i64 %58
  store i32 -1, i32* %59, align 4
  br label %60

; <label>:60:                                     ; preds = %55, %52
  br label %61

; <label>:61:                                     ; preds = %60
  %62 = load i32, i32* %7, align 4
  %63 = icmp eq i32 %62, 2
  br i1 %63, label %64, label %69

; <label>:64:                                     ; preds = %61
  %65 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.5, i32 0, i32 0))
  %66 = load i32, i32* %5, align 4
  %67 = sext i32 %66 to i64
  %68 = getelementptr inbounds [1 x i32], [1 x i32]* %6, i64 0, i64 %67
  store i32 -2, i32* %68, align 4
  br label %69

; <label>:69:                                     ; preds = %64, %61
  br label %70

; <label>:70:                                     ; preds = %69
  %71 = load i32, i32* %3, align 4
  %72 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.8, i32 0, i32 0), i32 %71)
  ret i32 0
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @caller_fn() #0 {
  %1 = alloca %struct.packet*, align 8
  %2 = call noalias i8* @malloc(i64 16) #1
  %3 = bitcast i8* %2 to %struct.packet*
  store %struct.packet* %3, %struct.packet** %1, align 8
  %4 = load %struct.packet*, %struct.packet** %1, align 8
  %5 = getelementptr inbounds %struct.packet, %struct.packet* %4, i32 0, i32 0
  store i32 1000, i32* %5, align 4
  %6 = load %struct.packet*, %struct.packet** %1, align 8
  %7 = getelementptr inbounds %struct.packet, %struct.packet* %6, i32 0, i32 1
  store i32 1, i32* %7, align 4
  %8 = load %struct.packet*, %struct.packet** %1, align 8
  %9 = getelementptr inbounds %struct.packet, %struct.packet* %8, i32 0, i32 2
  store i32 200, i32* %9, align 4
  %10 = load %struct.packet*, %struct.packet** %1, align 8
  %11 = getelementptr inbounds %struct.packet, %struct.packet* %10, i32 0, i32 3
  store i32 300, i32* %11, align 4
  %12 = load %struct.packet*, %struct.packet** %1, align 8
  %13 = call i32 @process_packet(%struct.packet* %12)
  %14 = load %struct.packet*, %struct.packet** %1, align 8
  %15 = bitcast %struct.packet* %14 to i8*
  call void @free(i8* %15) #1
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #3

; Function Attrs: nounwind
declare void @free(i8*) #3

attributes #0 = { noinline nounwind optnone sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 8.0.1 (tags/RELEASE_801/final)"}
