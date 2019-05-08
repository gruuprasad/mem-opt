; ModuleID = 'lookup.c'
source_filename = "lookup.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.packet = type { i32, i64 }

@hashes = common dso_local global [8192 x i32] zeroinitializer, align 16
@packet_buffer = common dso_local global [48 x %struct.packet] zeroinitializer, align 16
@.str = private unnamed_addr constant [10 x i8] c"expensive\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [9 x i8] c"lookup.c\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.3 = private unnamed_addr constant [10 x i8] c"access %d\00", align 1
@.str.4 = private unnamed_addr constant [8 x i8] c"loop 2\0A\00", align 1
@.str.5 = private unnamed_addr constant [10 x i8] c"tas_batch\00", section "llvm.metadata"
@expected = common dso_local global [48 x i32] zeroinitializer, align 16
@llvm.global.annotations = appending global [3 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (void (%struct.packet*)* @hash_lookup_loopSplit to i8*), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.1, i32 0, i32 0), i32 44 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void (%struct.packet*)* @hash_lookup_v2 to i8*), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.1, i32 0, i32 0), i32 59 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void (%struct.packet*)* @hash_lookup to i8*), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.1, i32 0, i32 0), i32 73 }], section "llvm.metadata"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @fill_hashTable() #0 {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 8192
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %1 = load i32, i32* %i, align 4
  %2 = load i32, i32* %i, align 4
  %idxprom = sext i32 %2 to i64
  %arrayidx = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %idxprom
  store i32 %1, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %3 = load i32, i32* %i, align 4
  %inc = add nsw i32 %3, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @fill_packet_buffer(%struct.packet** %pkts, i32 %n) #0 {
entry:
  %pkts.addr = alloca %struct.packet**, align 8
  %n.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store %struct.packet** %pkts, %struct.packet*** %pkts.addr, align 8
  store i32 %n, i32* %n.addr, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, i32* %i, align 4
  %1 = load i32, i32* %n.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load i32, i32* %i, align 4
  %3 = load i32, i32* %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %idxprom
  %id = getelementptr inbounds %struct.packet, %struct.packet* %arrayidx, i32 0, i32 0
  store i32 %2, i32* %id, align 16
  %4 = load i32, i32* %i, align 4
  %idxprom1 = sext i32 %4 to i64
  %arrayidx2 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %idxprom1
  %size = getelementptr inbounds %struct.packet, %struct.packet* %arrayidx2, i32 0, i32 1
  store i64 32, i64* %size, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %5 = load i32, i32* %i, align 4
  %inc = add nsw i32 %5, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @test_loop_function() #0 {
entry:
  %M = alloca i32, align 4
  %sum = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 10, i32* %M, align 4
  store i32 0, i32* %sum, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc4, %entry
  %0 = load i32, i32* %i, align 4
  %1 = load i32, i32* %M, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end6

for.body:                                         ; preds = %for.cond
  store i32 0, i32* %j, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %2 = load i32, i32* %j, align 4
  %3 = load i32, i32* %M, align 4
  %cmp2 = icmp slt i32 %2, %3
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %4 = load i32, i32* %sum, align 4
  %5 = load i32, i32* %j, align 4
  %add = add nsw i32 %4, %5
  store i32 %add, i32* %sum, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %6 = load i32, i32* %j, align 4
  %inc = add nsw i32 %6, 1
  store i32 %inc, i32* %j, align 4
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %for.inc4

for.inc4:                                         ; preds = %for.end
  %7 = load i32, i32* %i, align 4
  %inc5 = add nsw i32 %7, 1
  store i32 %inc5, i32* %i, align 4
  br label %for.cond

for.end6:                                         ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @hash_lookup_loopSplit(%struct.packet* %pkt) #0 {
entry:
  %pkt.addr = alloca %struct.packet*, align 8
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  %h = alloca i32, align 4
  %i4 = alloca i32, align 4
  store %struct.packet* %pkt, %struct.packet** %pkt.addr, align 8
  %pkt.addr1 = bitcast %struct.packet** %pkt.addr to i8*
  call void @llvm.var.annotation(i8* %pkt.addr1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.1, i32 0, i32 0), i32 44)
  store i32 48, i32* %n, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, i32* %i, align 4
  %1 = load i32, i32* %n, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load %struct.packet*, %struct.packet** %pkt.addr, align 8
  %id = getelementptr inbounds %struct.packet, %struct.packet* %2, i32 0, i32 0
  %3 = load i32, i32* %id, align 8
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %idxprom
  %4 = load i32, i32* %arrayidx, align 4
  store i32 %4, i32* %h, align 4
  %5 = load i32, i32* %h, align 4
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i32 0, i32 0), i32 %5)
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %6 = load i32, i32* %i, align 4
  %inc = add nsw i32 %6, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %7 = load %struct.packet*, %struct.packet** %pkt.addr, align 8
  %id2 = getelementptr inbounds %struct.packet, %struct.packet* %7, i32 0, i32 0
  %8 = load i32, i32* %id2, align 8
  %call3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.3, i32 0, i32 0), i32 %8)
  store i32 0, i32* %i4, align 4
  br label %for.cond5

for.cond5:                                        ; preds = %for.inc9, %for.end
  %9 = load i32, i32* %i4, align 4
  %10 = load i32, i32* %n, align 4
  %cmp6 = icmp slt i32 %9, %10
  br i1 %cmp6, label %for.body7, label %for.end11

for.body7:                                        ; preds = %for.cond5
  %call8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.4, i32 0, i32 0))
  br label %for.inc9

for.inc9:                                         ; preds = %for.body7
  %11 = load i32, i32* %i4, align 4
  %inc10 = add nsw i32 %11, 1
  store i32 %inc10, i32* %i4, align 4
  br label %for.cond5

for.end11:                                        ; preds = %for.cond5
  ret void
}

; Function Attrs: nounwind
declare void @llvm.var.annotation(i8*, i8*, i8*, i32) #1

declare dso_local i32 @printf(i8*, ...) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @hash_lookup_v2(%struct.packet* %pkt) #0 {
entry:
  %pkt.addr = alloca %struct.packet*, align 8
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  %h = alloca i32, align 4
  %i1 = alloca i32, align 4
  store %struct.packet* %pkt, %struct.packet** %pkt.addr, align 8
  store i32 48, i32* %n, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc6, %entry
  %0 = load i32, i32* %i, align 4
  %1 = load i32, i32* %n, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end8

for.body:                                         ; preds = %for.cond
  %2 = load %struct.packet*, %struct.packet** %pkt.addr, align 8
  %id = getelementptr inbounds %struct.packet, %struct.packet* %2, i32 0, i32 0
  %3 = load i32, i32* %id, align 8
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %idxprom
  %4 = load i32, i32* %arrayidx, align 4
  store i32 %4, i32* %h, align 4
  %5 = load i32, i32* %h, align 4
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i32 0, i32 0), i32 %5)
  store i32 0, i32* %i1, align 4
  br label %for.cond2

for.cond2:                                        ; preds = %for.inc, %for.body
  %6 = load i32, i32* %i1, align 4
  %cmp3 = icmp slt i32 %6, 10
  br i1 %cmp3, label %for.body4, label %for.end

for.body4:                                        ; preds = %for.cond2
  %call5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.4, i32 0, i32 0))
  br label %for.inc

for.inc:                                          ; preds = %for.body4
  %7 = load i32, i32* %i1, align 4
  %inc = add nsw i32 %7, 1
  store i32 %inc, i32* %i1, align 4
  br label %for.cond2

for.end:                                          ; preds = %for.cond2
  br label %for.inc6

for.inc6:                                         ; preds = %for.end
  %8 = load i32, i32* %i, align 4
  %inc7 = add nsw i32 %8, 1
  store i32 %inc7, i32* %i, align 4
  br label %for.cond

for.end8:                                         ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @hash_lookup(%struct.packet* %pkt) #0 {
entry:
  %pkt.addr = alloca %struct.packet*, align 8
  %h = alloca i32, align 4
  %p = alloca %struct.packet*, align 8
  store %struct.packet* %pkt, %struct.packet** %pkt.addr, align 8
  %pkt.addr1 = bitcast %struct.packet** %pkt.addr to i8*
  call void @llvm.var.annotation(i8* %pkt.addr1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.1, i32 0, i32 0), i32 73)
  %p2 = bitcast %struct.packet** %p to i8*
  call void @llvm.var.annotation(i8* %p2, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.1, i32 0, i32 0), i32 75)
  %0 = load %struct.packet*, %struct.packet** %pkt.addr, align 8
  %id = getelementptr inbounds %struct.packet, %struct.packet* %0, i32 0, i32 0
  %1 = load i32, i32* %id, align 8
  %idxprom = sext i32 %1 to i64
  %arrayidx = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %idxprom
  %2 = load i32, i32* %arrayidx, align 4
  store i32 %2, i32* %h, align 4
  %3 = load i32, i32* %h, align 4
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i32 0, i32 0), i32 %3)
  %call3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.4, i32 0, i32 0))
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %pkts_buf = alloca %struct.packet**, align 8
  %i = alloca i32, align 4
  %i2 = alloca i32, align 4
  %i11 = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %call = call noalias i8* @malloc(i64 384) #1
  %0 = bitcast i8* %call to %struct.packet**
  store %struct.packet** %0, %struct.packet*** %pkts_buf, align 8
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %1 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %1, 48
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %call1 = call noalias i8* @malloc(i64 16) #1
  %2 = bitcast i8* %call1 to %struct.packet*
  %3 = load %struct.packet**, %struct.packet*** %pkts_buf, align 8
  %4 = load i32, i32* %i, align 4
  %idxprom = sext i32 %4 to i64
  %arrayidx = getelementptr inbounds %struct.packet*, %struct.packet** %3, i64 %idxprom
  store %struct.packet* %2, %struct.packet** %arrayidx, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %5 = load i32, i32* %i, align 4
  %inc = add nsw i32 %5, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  call void @fill_hashTable()
  %6 = load %struct.packet**, %struct.packet*** %pkts_buf, align 8
  call void @fill_packet_buffer(%struct.packet** %6, i32 48)
  store i32 0, i32* %i2, align 4
  br label %for.cond3

for.cond3:                                        ; preds = %for.inc8, %for.end
  %7 = load i32, i32* %i2, align 4
  %cmp4 = icmp slt i32 %7, 48
  br i1 %cmp4, label %for.body5, label %for.end10

for.body5:                                        ; preds = %for.cond3
  %8 = load %struct.packet**, %struct.packet*** %pkts_buf, align 8
  %9 = load i32, i32* %i2, align 4
  %idxprom6 = sext i32 %9 to i64
  %arrayidx7 = getelementptr inbounds %struct.packet*, %struct.packet** %8, i64 %idxprom6
  %10 = load %struct.packet*, %struct.packet** %arrayidx7, align 8
  call void @hash_lookup(%struct.packet* %10)
  br label %for.inc8

for.inc8:                                         ; preds = %for.body5
  %11 = load i32, i32* %i2, align 4
  %inc9 = add nsw i32 %11, 1
  store i32 %inc9, i32* %i2, align 4
  br label %for.cond3

for.end10:                                        ; preds = %for.cond3
  store i32 0, i32* %i11, align 4
  br label %for.cond12

for.cond12:                                       ; preds = %for.inc17, %for.end10
  %12 = load i32, i32* %i11, align 4
  %cmp13 = icmp slt i32 %12, 48
  br i1 %cmp13, label %for.body14, label %for.end19

for.body14:                                       ; preds = %for.cond12
  %13 = load %struct.packet**, %struct.packet*** %pkts_buf, align 8
  %14 = load i32, i32* %i11, align 4
  %idxprom15 = sext i32 %14 to i64
  %arrayidx16 = getelementptr inbounds %struct.packet*, %struct.packet** %13, i64 %idxprom15
  %15 = load %struct.packet*, %struct.packet** %arrayidx16, align 8
  %16 = bitcast %struct.packet* %15 to i8*
  call void @free(i8* %16) #1
  br label %for.inc17

for.inc17:                                        ; preds = %for.body14
  %17 = load i32, i32* %i11, align 4
  %inc18 = add nsw i32 %17, 1
  store i32 %inc18, i32* %i11, align 4
  br label %for.cond12

for.end19:                                        ; preds = %for.cond12
  %18 = load %struct.packet**, %struct.packet*** %pkts_buf, align 8
  %19 = bitcast %struct.packet** %18 to i8*
  call void @free(i8* %19) #1
  ret i32 0
}

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) #3

; Function Attrs: nounwind
declare dso_local void @free(i8*) #3

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (trunk 341799)"}
