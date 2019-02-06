; ModuleID = 'lookup.ll'
source_filename = "lookup.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.packet = type { i32, i64 }

@hashes = common dso_local global [8192 x i32] zeroinitializer, align 16
@packet_buffer = common dso_local global [48 x %struct.packet] zeroinitializer, align 16
@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.1 = private unnamed_addr constant [10 x i8] c"tas_batch\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [9 x i8] c"lookup.c\00", section "llvm.metadata"
@expected = common dso_local global [48 x i32] zeroinitializer, align 16
@llvm.global.annotations = appending global [1 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (void (%struct.packet*)* @hash_lookup to i8*), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.2, i32 0, i32 0), i32 41 }], section "llvm.metadata"

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @fill_hashTable() #0 {
  %1 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  br label %2

; <label>:2:                                      ; preds = %10, %0
  %3 = load i32, i32* %1, align 4
  %4 = icmp slt i32 %3, 8192
  br i1 %4, label %5, label %13

; <label>:5:                                      ; preds = %2
  %6 = load i32, i32* %1, align 4
  %7 = load i32, i32* %1, align 4
  %8 = sext i32 %7 to i64
  %9 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %8
  store i32 %6, i32* %9, align 4
  br label %10

; <label>:10:                                     ; preds = %5
  %11 = load i32, i32* %1, align 4
  %12 = add nsw i32 %11, 1
  store i32 %12, i32* %1, align 4
  br label %2

; <label>:13:                                     ; preds = %2
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @fill_packet_buffer(%struct.packet**, i32) #0 {
  %3 = alloca %struct.packet**, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store %struct.packet** %0, %struct.packet*** %3, align 8
  store i32 %1, i32* %4, align 4
  store i32 0, i32* %5, align 4
  br label %6

; <label>:6:                                      ; preds = %20, %2
  %7 = load i32, i32* %5, align 4
  %8 = load i32, i32* %4, align 4
  %9 = icmp slt i32 %7, %8
  br i1 %9, label %10, label %23

; <label>:10:                                     ; preds = %6
  %11 = load i32, i32* %5, align 4
  %12 = load i32, i32* %5, align 4
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %13
  %15 = getelementptr inbounds %struct.packet, %struct.packet* %14, i32 0, i32 0
  store i32 %11, i32* %15, align 16
  %16 = load i32, i32* %5, align 4
  %17 = sext i32 %16 to i64
  %18 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %17
  %19 = getelementptr inbounds %struct.packet, %struct.packet* %18, i32 0, i32 1
  store i64 32, i64* %19, align 8
  br label %20

; <label>:20:                                     ; preds = %10
  %21 = load i32, i32* %5, align 4
  %22 = add nsw i32 %21, 1
  store i32 %22, i32* %5, align 4
  br label %6

; <label>:23:                                     ; preds = %6
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @test_loop_function() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 10, i32* %1, align 4
  store i32 0, i32* %2, align 4
  store i32 0, i32* %3, align 4
  br label %5

; <label>:5:                                      ; preds = %22, %0
  %6 = load i32, i32* %3, align 4
  %7 = load i32, i32* %1, align 4
  %8 = icmp slt i32 %6, %7
  br i1 %8, label %9, label %25

; <label>:9:                                      ; preds = %5
  store i32 0, i32* %4, align 4
  br label %10

; <label>:10:                                     ; preds = %18, %9
  %11 = load i32, i32* %4, align 4
  %12 = load i32, i32* %1, align 4
  %13 = icmp slt i32 %11, %12
  br i1 %13, label %14, label %21

; <label>:14:                                     ; preds = %10
  %15 = load i32, i32* %2, align 4
  %16 = load i32, i32* %4, align 4
  %17 = add nsw i32 %15, %16
  store i32 %17, i32* %2, align 4
  br label %18

; <label>:18:                                     ; preds = %14
  %19 = load i32, i32* %4, align 4
  %20 = add nsw i32 %19, 1
  store i32 %20, i32* %4, align 4
  br label %10

; <label>:21:                                     ; preds = %10
  br label %22

; <label>:22:                                     ; preds = %21
  %23 = load i32, i32* %3, align 4
  %24 = add nsw i32 %23, 1
  store i32 %24, i32* %3, align 4
  br label %5

; <label>:25:                                     ; preds = %5
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @hash_lookup(%struct.packet*) #0 {
  %2 = alloca %struct.packet*, align 8
  %3 = alloca i32, align 4
  store %struct.packet* %0, %struct.packet** %2, align 8
  %4 = load %struct.packet*, %struct.packet** %2, align 8
  %5 = getelementptr inbounds %struct.packet, %struct.packet* %4, i32 0, i32 0
  %6 = load i32, i32* %5, align 8
  %7 = sext i32 %6 to i64
  %8 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %7
  %9 = load i32, i32* %8, align 4
  store i32 %9, i32* %3, align 4
  %10 = load i32, i32* %3, align 4
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32 %10)
  ret void
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca %struct.packet**, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  %6 = call noalias i8* @malloc(i64 384) #3
  %7 = bitcast i8* %6 to %struct.packet**
  store %struct.packet** %7, %struct.packet*** %2, align 8
  store i32 0, i32* %3, align 4
  br label %8

; <label>:8:                                      ; preds = %18, %0
  %9 = load i32, i32* %3, align 4
  %10 = icmp slt i32 %9, 48
  br i1 %10, label %11, label %21

; <label>:11:                                     ; preds = %8
  %12 = call noalias i8* @malloc(i64 16) #3
  %13 = bitcast i8* %12 to %struct.packet*
  %14 = load %struct.packet**, %struct.packet*** %2, align 8
  %15 = load i32, i32* %3, align 4
  %16 = sext i32 %15 to i64
  %17 = getelementptr inbounds %struct.packet*, %struct.packet** %14, i64 %16
  store %struct.packet* %13, %struct.packet** %17, align 8
  br label %18

; <label>:18:                                     ; preds = %11
  %19 = load i32, i32* %3, align 4
  %20 = add nsw i32 %19, 1
  store i32 %20, i32* %3, align 4
  br label %8

; <label>:21:                                     ; preds = %8
  call void @fill_hashTable()
  %22 = load %struct.packet**, %struct.packet*** %2, align 8
  call void @fill_packet_buffer(%struct.packet** %22, i32 48)
  store i32 0, i32* %4, align 4
  br label %23

; <label>:23:                                     ; preds = %32, %21
  %24 = load i32, i32* %4, align 4
  %25 = icmp slt i32 %24, 48
  br i1 %25, label %26, label %35

; <label>:26:                                     ; preds = %23
  %27 = load %struct.packet**, %struct.packet*** %2, align 8
  %28 = load i32, i32* %4, align 4
  %29 = sext i32 %28 to i64
  %30 = getelementptr inbounds %struct.packet*, %struct.packet** %27, i64 %29
  %31 = load %struct.packet*, %struct.packet** %30, align 8
  call void @hash_lookup(%struct.packet* %31)
  br label %32

; <label>:32:                                     ; preds = %26
  %33 = load i32, i32* %4, align 4
  %34 = add nsw i32 %33, 1
  store i32 %34, i32* %4, align 4
  br label %23

; <label>:35:                                     ; preds = %23
  store i32 0, i32* %5, align 4
  br label %36

; <label>:36:                                     ; preds = %46, %35
  %37 = load i32, i32* %5, align 4
  %38 = icmp slt i32 %37, 48
  br i1 %38, label %39, label %49

; <label>:39:                                     ; preds = %36
  %40 = load %struct.packet**, %struct.packet*** %2, align 8
  %41 = load i32, i32* %5, align 4
  %42 = sext i32 %41 to i64
  %43 = getelementptr inbounds %struct.packet*, %struct.packet** %40, i64 %42
  %44 = load %struct.packet*, %struct.packet** %43, align 8
  %45 = bitcast %struct.packet* %44 to i8*
  call void @free(i8* %45) #3
  br label %46

; <label>:46:                                     ; preds = %39
  %47 = load i32, i32* %5, align 4
  %48 = add nsw i32 %47, 1
  store i32 %48, i32* %5, align 4
  br label %36

; <label>:49:                                     ; preds = %36
  %50 = load %struct.packet**, %struct.packet*** %2, align 8
  %51 = bitcast %struct.packet** %50 to i8*
  call void @free(i8* %51) #3
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #2

; Function Attrs: nounwind
declare void @free(i8*) #2

attributes #0 = { noinline nounwind optnone sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 7.0.1 (tags/RELEASE_701/final)"}
