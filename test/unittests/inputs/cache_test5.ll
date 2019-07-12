; ModuleID = 'cache_test5.c'
source_filename = "cache_test5.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.pl_memory = type { [32 x %struct.packet], [32 x %struct.packet_state] }
%struct.packet = type { i32, i32, i32, i32 }
%struct.packet_state = type { i64, i64, i64, i64, %struct.metadata, i64, i64, i64, i64, i64, i64, i64, i64, i64, i64, i64, i64, %struct.metadata }
%struct.metadata = type { i64, i64, i64, i64 }

@.str = private unnamed_addr constant [29 x i8] c"Size of struct packet = %lu\0A\00", align 1
@.str.1 = private unnamed_addr constant [35 x i8] c"Size of struct packet_state = %lu\0A\00", align 1
@.str.2 = private unnamed_addr constant [31 x i8] c"Size of struct metadata = %lu\0A\00", align 1
@.str.3 = private unnamed_addr constant [32 x i8] c"Size of struct pl_memory = %lu\0A\00", align 1

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @test_fn(%struct.pl_memory*) #0 {
  %2 = alloca %struct.pl_memory*, align 8
  store %struct.pl_memory* %0, %struct.pl_memory** %2, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str, i32 0, i32 0), i64 16)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.1, i32 0, i32 0), i64 192)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.2, i32 0, i32 0), i64 32)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.3, i32 0, i32 0), i64 6656)
  %7 = load %struct.pl_memory*, %struct.pl_memory** %2, align 8
  %8 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %7, i32 0, i32 0
  %9 = getelementptr inbounds [32 x %struct.packet], [32 x %struct.packet]* %8, i64 0, i64 1
  %10 = getelementptr inbounds %struct.packet, %struct.packet* %9, i32 0, i32 0
  store i32 3, i32* %10, align 8
  %11 = load %struct.pl_memory*, %struct.pl_memory** %2, align 8
  %12 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %11, i32 0, i32 0
  %13 = getelementptr inbounds [32 x %struct.packet], [32 x %struct.packet]* %12, i64 0, i64 1
  %14 = getelementptr inbounds %struct.packet, %struct.packet* %13, i32 0, i32 1
  store i32 2, i32* %14, align 4
  %15 = load %struct.pl_memory*, %struct.pl_memory** %2, align 8
  %16 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %15, i32 0, i32 0
  %17 = getelementptr inbounds [32 x %struct.packet], [32 x %struct.packet]* %16, i64 0, i64 1
  %18 = getelementptr inbounds %struct.packet, %struct.packet* %17, i32 0, i32 2
  store i32 4, i32* %18, align 8
  %19 = load %struct.pl_memory*, %struct.pl_memory** %2, align 8
  %20 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %19, i32 0, i32 0
  %21 = getelementptr inbounds [32 x %struct.packet], [32 x %struct.packet]* %20, i64 0, i64 1
  %22 = getelementptr inbounds %struct.packet, %struct.packet* %21, i32 0, i32 3
  store i32 1, i32* %22, align 4
  %23 = load %struct.pl_memory*, %struct.pl_memory** %2, align 8
  %24 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %23, i32 0, i32 0
  %25 = getelementptr inbounds [32 x %struct.packet], [32 x %struct.packet]* %24, i64 0, i64 15
  %26 = getelementptr inbounds %struct.packet, %struct.packet* %25, i32 0, i32 3
  store i32 15, i32* %26, align 4
  %27 = load %struct.pl_memory*, %struct.pl_memory** %2, align 8
  %28 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %27, i32 0, i32 1
  %29 = getelementptr inbounds [32 x %struct.packet_state], [32 x %struct.packet_state]* %28, i64 0, i64 0
  %30 = getelementptr inbounds %struct.packet_state, %struct.packet_state* %29, i32 0, i32 0
  store i64 100, i64* %30, align 8
  %31 = load %struct.pl_memory*, %struct.pl_memory** %2, align 8
  %32 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %31, i32 0, i32 1
  %33 = getelementptr inbounds [32 x %struct.packet_state], [32 x %struct.packet_state]* %32, i64 0, i64 2
  %34 = getelementptr inbounds %struct.packet_state, %struct.packet_state* %33, i32 0, i32 0
  store i64 100, i64* %34, align 8
  %35 = load %struct.pl_memory*, %struct.pl_memory** %2, align 8
  %36 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %35, i32 0, i32 1
  %37 = getelementptr inbounds [32 x %struct.packet_state], [32 x %struct.packet_state]* %36, i64 0, i64 2
  %38 = getelementptr inbounds %struct.packet_state, %struct.packet_state* %37, i32 0, i32 1
  store i64 100, i64* %38, align 8
  %39 = load %struct.pl_memory*, %struct.pl_memory** %2, align 8
  %40 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %39, i32 0, i32 1
  %41 = getelementptr inbounds [32 x %struct.packet_state], [32 x %struct.packet_state]* %40, i64 0, i64 2
  %42 = getelementptr inbounds %struct.packet_state, %struct.packet_state* %41, i32 0, i32 4
  %43 = getelementptr inbounds %struct.metadata, %struct.metadata* %42, i32 0, i32 1
  store i64 300, i64* %43, align 8
  ret void
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @caller_fn() #0 {
  %1 = alloca %struct.pl_memory*, align 8
  %2 = call noalias i8* @malloc(i64 6656) #3
  %3 = bitcast i8* %2 to %struct.pl_memory*
  store %struct.pl_memory* %3, %struct.pl_memory** %1, align 8
  %4 = load %struct.pl_memory*, %struct.pl_memory** %1, align 8
  call void @test_fn(%struct.pl_memory* %4)
  %5 = load %struct.pl_memory*, %struct.pl_memory** %1, align 8
  %6 = bitcast %struct.pl_memory* %5 to i8*
  call void @free(i8* %6) #3
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #2

; Function Attrs: nounwind
declare void @free(i8*) #2

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @main() #0 {
  %1 = call i32 @caller_fn()
  ret i32 0
}

attributes #0 = { noinline nounwind optnone sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
